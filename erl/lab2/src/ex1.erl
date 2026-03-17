%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 17. mar 2026 11:48
%%%-------------------------------------------------------------------
-module(ex1).
-author("julia").

%% API
-export([qs/1, random_elems/3, compare_speeds/3]).

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
