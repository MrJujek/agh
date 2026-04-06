%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 31. mar 2026 11:45
%%%-------------------------------------------------------------------
-module(ping_pong).
-author("julia").

%% API
-export([start/0, stop/0, play/1]).

ping_loop(Sum) ->
  receive
    stop ->
      io:format("Ping stop, Sum: ~p~n", [Sum]);
    0 ->
      io:format("Ping 0~n"),
      ping_loop(Sum);
    N when is_integer(N), N > 0 ->
      io:format("Ping ~p~n", [N]),
      NewSum = N + Sum,
      timer:sleep(50),
      pong ! (N - 1),
      ping_loop(NewSum)
  after 10000 ->
    io:format("Ping 10s, Sum: ~p~n", [Sum])
  end.

pong_loop() ->
  receive
    stop ->
      io:format("Pong stop~n");
    0 ->
      io:format("Pong 0~n"),
      pong_loop();
    N when is_integer(N), N > 0 ->
      io:format("Pong ~p~n", [N]),
      timer:sleep(50),
      ping ! (N - 1),
      pong_loop()
  after 10000 ->
    io:format("Pong 10s~n")
  end.

start() ->
  register(ping, spawn(fun() -> ping_loop(0) end)),
  register(pong, spawn(fun() -> pong_loop() end)),
  ok.

stop() ->
  spawn(fun() -> ping ! stop end),
  spawn(fun() -> pong ! stop end),
  ok.

play(N) when is_integer(N), N > 0 ->
  ping ! N,
  ok.