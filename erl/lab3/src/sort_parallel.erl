%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 31. mar 2026 12:18
%%%-------------------------------------------------------------------
-module(sort_parallel).
-author("julia").

%% API
-export([qs/1, random_elems/3, compare_speeds/3, generate_data/1, sort_lists/1, qs_proc/2, sort_lists_proc/1, run_test/1]).

less_than([], _Arg) -> [];
less_than(List, Arg) -> [X || X <- List, X < Arg].

grt_eq_than([], _Arg) -> [];
grt_eq_than(List, Arg) -> [X || X <- List, X >= Arg].

qs([]) -> [];
qs([Pivot|Tail]) -> qs( less_than(Tail,Pivot) ) ++ [Pivot] ++ qs( grt_eq_than(Tail,Pivot) ).

random_elems(N, Min, Max) -> [rand:uniform(Max-Min)+Min-1 || _ <- lists:seq(1, N)].

compare_speeds(List, Fun1, Fun2) ->
  {Time1, _} = timer:tc(Fun1, [List]),
  {Time2, _} = timer:tc(Fun2, [List]),
  abs(Time1 - Time2).

generate_data(N) ->
  [random_elems(N, 1, 10000) || _ <- lists:seq(1, 1000)].

sort_lists(Lists) ->
  [qs(L) || L <- Lists].

qs_proc(List, Pid) ->
  Pid ! {self(), qs(List)}.

sort_lists_proc(Lists) ->
  Parent = self(),
  Pids = [spawn(fun() -> qs_proc(L, Parent) end) || L <- Lists],
  collect_results(Pids).

collect_results([]) -> [];
collect_results([Pid | Tail]) ->
  receive
    {Pid, SortedList} ->
      [SortedList | collect_results(Tail)]
  end.

run_test(N) ->
  io:format("Generowanie danych...~n"),
  Data = generate_data(N),

  io:format("Rozpoczynam sortowanie sekwencyjne...~n"),
  {TimeSeq, _} = timer:tc(fun() -> sort_lists(Data) end),
  io:format("Czas sekwencyjny: ~p mikrosekund (~p sekund)~n", [TimeSeq, TimeSeq / 1000000.0]),

  io:format("Rozpoczynam sortowanie rownolegle...~n"),
  {TimePar, _} = timer:tc(fun() -> sort_lists_proc(Data) end),
  io:format("Czas rownolegly: ~p mikrosekund (~p sekund)~n", [TimePar, TimePar / 1000000.0]).