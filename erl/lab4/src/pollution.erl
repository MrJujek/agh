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
-export([create_monitor/0, add_station/3, add_value/5, remove_value/4, get_one_value/4, get_station_min/3, get_station_mean/3, get_daily_mean/3, get_daily_over_limit/4, get_air_quality_index/3]).

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
      case maps:is_key(Key, Measurements) of
        true -> Monitor#{measurements => maps:remove(Key, Measurements)};
        false -> {error, "Measurement not found"}
      end;
    {error, Msg} -> {error, Msg}
  end.

get_one_value(StationId, Date, Type, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Key = {Coords, Date, Type},
      case maps:find(Key, Measurements) of
        {ok, Value} -> Value;
        error -> {error, "Measurement not found"}
      end;
    {error, Msg} -> {error, Msg}
  end.

get_station_min(StationId, Type, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Values = [V || {{C, _Date, T}, V} <- maps:to_list(Measurements), C == Coords, T == Type],
      case Values of
        [] -> {error, "No measurements found"};
        _ -> lists:min(Values)
      end;
    {error, Msg} -> {error, Msg}
  end.

get_day({Y, M, D}) -> {Y, M, D};
get_day({{Y, M, D}, _Time}) -> {Y, M, D}.

get_station_mean(StationId, Type, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Values = [V || {{C, _Date, T}, V} <- maps:to_list(Measurements), C == Coords, T == Type],
      case Values of
        [] -> {error, "No measurements found"};
        _ -> lists:sum(Values) / length(Values)
      end;
    {error, Msg} -> {error, Msg}
  end.

get_daily_mean(Type, Date, Monitor) ->
  #{measurements := Measurements} = Monitor,
  TargetDay = get_day(Date),
  Values = [V || {{_Coords, D, T}, V} <- maps:to_list(Measurements), T == Type, get_day(D) == TargetDay],
  case Values of
    [] -> {error, "No measurements found"};
    _ -> lists:sum(Values) / length(Values)
  end.

get_daily_over_limit(Type, Date, Limit, Monitor) ->
  #{measurements := Measurements} = Monitor,
  TargetDay = get_day(Date),
  OverLimitStations = [Coords || {{Coords, D, T}, V} <- maps:to_list(Measurements), T == Type, get_day(D) == TargetDay, V > Limit],
  length(lists:usort(OverLimitStations)).

get_norm("PM10") -> 50;
get_norm("PM2.5") -> 30;
get_norm("PM25") -> 30;
get_norm(_) -> 100.

get_air_quality_index(StationId, Date, Monitor) ->
  case get_coords(StationId, Monitor) of
    {ok, Coords} ->
      #{measurements := Measurements} = Monitor,
      Acc = [ (V / get_norm(T)) * 100 || {{C, D, T}, V} <- maps:to_list(Measurements), C == Coords, D == Date ],
      case Acc of
        [] -> {error, "No measurements found"};
        _ -> lists:max(Acc)
      end;
    {error, Msg} -> {error, Msg}
  end.