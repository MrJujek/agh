%%%-------------------------------------------------------------------
%%% @author julo
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 13. mar 2026 20:33
%%%-------------------------------------------------------------------
-module(ex2).
-author("julo").

%% API
-export([power/2, contains/2, duplicate_elements/1, sum_floats/1]).

power(_, 0) -> 1;
power(Base, Exponent) when Exponent > 0 ->
  Base * power(Base, Exponent - 1).

contains([], _) -> false;
contains([Value | _], Value) -> true;
contains([_ | Tail], Value) -> contains(Tail, Value).

duplicate_elements([]) -> [];
duplicate_elements([Head | Tail]) -> [Head, Head | duplicate_elements(Tail)].

sum_floats([]) -> 0.0;
sum_floats([Head | Tail]) when is_float(Head) ->
  Head + sum_floats(Tail);
sum_floats([_ | Tail]) ->
  sum_floats(Tail).