%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 10. kwi 2026 15:48
%%%-------------------------------------------------------------------
-module(pollution_server).
-author("julia").

%% API
-export([start/0, stop/0, init/0]).
-export([add_station/2, add_value/4, remove_value/3, get_one_value/3, get_station_min/2, get_station_mean/2, get_daily_mean/2, get_daily_over_limit/3, get_air_quality_index/2]).

start() ->
  register(pollution_server, spawn(pollution_server, init, [])).

stop() ->
  pollution_server ! stop,
  ok.

init() ->
  Monitor = pollution:create_monitor(),
  loop(Monitor).

loop(Monitor) ->
  receive
    {request, Pid, {add_station, Name, Coords}} ->
      case pollution:add_station(Name, Coords, Monitor) of
        {error, Msg} ->
          Pid ! {reply, {error, Msg}},
          loop(Monitor);
        NewMonitor ->
          Pid ! {reply, ok},
          loop(NewMonitor)
      end;
    {request, Pid, {add_value, StationId, Date, Type, Value}} ->
      case pollution:add_value(StationId, Date, Type, Value, Monitor) of
        {error, Msg} ->
          Pid ! {reply, {error, Msg}},
          loop(Monitor);
        NewMonitor ->
          Pid ! {reply, ok},
          loop(NewMonitor)
      end;
    {request, Pid, {remove_value, StationId, Date, Type}} ->
      case pollution:remove_value(StationId, Date, Type, Monitor) of
        {error, Msg} ->
          Pid ! {reply, {error, Msg}},
          loop(Monitor);
        NewMonitor ->
          Pid ! {reply, ok},
          loop(NewMonitor)
      end;
    {request, Pid, {get_one_value, StationId, Date, Type}} ->
      Pid ! {reply, pollution:get_one_value(StationId, Date, Type, Monitor)},
      loop(Monitor);
    {request, Pid, {get_station_min, StationId, Type}} ->
      Pid ! {reply, pollution:get_station_min(StationId, Type, Monitor)},
      loop(Monitor);
    {request, Pid, {get_station_mean, StationId, Type}} ->
      Pid ! {reply, pollution:get_station_mean(StationId, Type, Monitor)},
      loop(Monitor);
    {request, Pid, {get_daily_mean, Type, Date}} ->
      Pid ! {reply, pollution:get_daily_mean(Type, Date, Monitor)},
      loop(Monitor);
    {request, Pid, {get_daily_over_limit, Type, Date, Limit}} ->
      Pid ! {reply, pollution:get_daily_over_limit(Type, Date, Limit, Monitor)},
      loop(Monitor);
    {request, Pid, {get_air_quality_index, StationId, Date}} ->
      Pid ! {reply, pollution:get_air_quality_index(StationId, Date, Monitor)},
      loop(Monitor);
    stop ->
      ok
  end.

call(Message) ->
  pollution_server ! {request, self(), Message},
  receive
    {reply, Reply} ->
      Reply
  end.

add_station(Name, Coords) ->
  call({add_station, Name, Coords}).

add_value(StationId, Date, Type, Value) ->
  call({add_value, StationId, Date, Type, Value}).

remove_value(StationId, Date, Type) ->
  call({remove_value, StationId, Date, Type}).

get_one_value(StationId, Date, Type) ->
  call({get_one_value, StationId, Date, Type}).

get_station_min(StationId, Type) ->
  call({get_station_min, StationId, Type}).

get_station_mean(StationId, Type) ->
  call({get_station_mean, StationId, Type}).

get_daily_mean(Type, Date) ->
  call({get_daily_mean, Type, Date}).

get_daily_over_limit(Type, Date, Limit) ->
  call({get_daily_over_limit, Type, Date, Limit}).

get_air_quality_index(StationId, Date) ->
  call({get_air_quality_index, StationId, Date}).
