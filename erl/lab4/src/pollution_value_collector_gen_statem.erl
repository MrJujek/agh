%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 23. kwi 2026 18:01
%%%-------------------------------------------------------------------
-module(pollution_value_collector_gen_statem).
-author("julia").

-behaviour(gen_statem).

%% API
-export([start_link/0, stop/0, set_station/1, add_value/3, store_data/0]).

%% gen_statem callbacks
-export([init/1, terminate/3, callback_mode/0]).

-export([idle/3, collecting/3]).

-define(SERVER, ?MODULE).

%%%===================================================================
%%% API
%%%===================================================================

%% @doc Creates a gen_statem process which calls Module:init/1 to
%% initialize. To ensure a synchronized start-up procedure, this
%% function does not return until Module:init/1 has returned.
start_link() ->
  gen_statem:start_link({local, ?SERVER}, ?MODULE, [], []).

stop() ->
  gen_statem:stop(?SERVER).

set_station(StationId) ->
  gen_statem:call(?SERVER, {set_station, StationId}).

add_value(Date, Type, Value) ->
  gen_statem:call(?SERVER, {add_value, Date, Type, Value}).

store_data() ->
  gen_statem:call(?SERVER, store_data).

%%%===================================================================
%%% gen_statem callbacks
%%%===================================================================

init([]) ->
  Data = #{station => undefined, measurements => []},
  {ok, idle, Data}.

callback_mode() ->
  state_functions.

idle({call, From}, {set_station, StationId}, Data) ->
  NewData = Data#{station => StationId, measurements => []},
  {next_state, collecting, NewData, [{reply, From, ok}]};

idle({call, From}, _Msg, _Data) ->
  {keep_state_and_data, [{reply, From, {error, "Wrong order of operations. Call set_station/1 first"}}]}.

collecting({call, From}, {add_value, Date, Type, Value}, Data = #{measurements := Measurements}) ->
  NewData = Data#{measurements => [{Date, Type, Value} | Measurements]},
  {keep_state, NewData, [{reply, From, ok}]};

collecting({call, From}, store_data, #{station := StationId, measurements := Measurements}) ->

  lists:foreach(
    fun({Date, Type, Value}) ->
      pollution_gen_server:addValue(StationId, Date, Type, Value)
    end,
    lists:reverse(Measurements)
  ),
  NewData = #{station => undefined, measurements => []},
  {next_state, idle, NewData, [{reply, From, ok}]};

collecting({call, From}, _Msg, _Data) ->
  {keep_state_and_data, [{reply, From, {error, "Expected store_data or add_value"}}]}.

%% @private
%% @doc This function is called by a gen_statem when it is about to
%% terminate. It should be the opposite of Module:init/1 and do any
%% necessary cleaning up. When it returns, the gen_statem terminates with
%% Reason. The return value is ignored.
terminate(_Reason, _State, _Data) ->
  ok.