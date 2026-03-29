%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 29. mar 2026 18:11
%%%-------------------------------------------------------------------
-module(pollution).
-author("julia").

%% API
-export([create_monitor/0, add_station/3, add_value/5, remove_value/4]).

create_monitor() ->
  #{
    stations_by_name => #{},
    stations_by_coords => #{},
    measurements => #{}
  }.

add_station(Name, Coords, Monitor) ->
  #{stations_by_name := ByName, stations_by_coords := ByCoords} = Monitor,
  case {maps:is_key(Name, ByName), maps:is_key(Coords, ByCoords)} of
    {true, _} -> {error, "Station with this name already exists"};
    {_, true} -> {error, "Station with these coordinates already exists"};
    {false, false} ->
      Monitor#{
        stations_by_name => maps:put(Name, Coords, ByName),
        stations_by_coords => maps:put(Coords, Name, ByCoords)
      }
  end.

get_coords(Name, #{stations_by_name := ByName}) when is_list(Name) ->
  case maps:find(Name, ByName) of
    {ok, Coords} -> {ok, Coords};
    error -> {error, "Station not found"}
  end;
get_coords(Coords, #{stations_by_coords := ByCoords}) when is_tuple(Coords) ->
  case maps:is_key(Coords, ByCoords) of
    true -> {ok, Coords};
    false -> {error, "Station not found"}
  end.

add_value(StationId, Date, Type, Value, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Key = {Coords, Date, Type},
      case maps:is_key(Key, Measurements) of
        true -> {error, "Measurement already exists"};
        false -> Monitor#{measurements => maps:put(Key, Value, Measurements)}
      end;
    {error, Msg} -> {error, Msg}
  end.

remove_value(StationId, Date, Type, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Key = {Coords, Date, Type},
      Monitor#{measurements => maps:remove(Key, Measurements)};
    {error, Msg} -> {error, Msg}
  end.