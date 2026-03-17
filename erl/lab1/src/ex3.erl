%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 15. mar 2026 21:20
%%%-------------------------------------------------------------------
-module(ex3).
-author("julia").

%% API
-export([getData/0, numberOfReadings/2, calculateMinAndMax/2, calculateMean/2]).

getData() ->
  [
    {"station_1", {{2026, 3, 15}, {10, 23, 43}}, [{pm10, 10.0}, {pm25, 5.0}]},
    {"station_2", {{2026, 3, 15}, {14, 29, 11}}, [{pm10, 12.0}, {pm25, 6.0}, {temp, 19.0}]},
    {"station_3", {{2026, 3, 15}, {12, 45, 44}}, [{pm1, 30.0}, {pm64, 20.0}]},
    {"station_4", {{2026, 3, 15}, {21, 04, 34}}, [{pm25, 10.0}, {press, 25.0}, {hum, 55.0}]}
  ].

numberOfReadings(Readings, Date) ->
  numberOfReadings(Readings, Date, 0).

numberOfReadings([], _Date, Acc) -> Acc;
numberOfReadings([{_, {Date, _}, _} | Tail], Date, Acc) ->
  numberOfReadings(Tail, Date, Acc + 1);
numberOfReadings([_ | Tail], Date, Acc) ->
  numberOfReadings(Tail, Date, Acc).

getValuesByType(Readings, Type) ->
  getValuesByType(Readings, Type, []).

getValuesByType([], _Type, Acc) ->
  Acc;
getValuesByType([{_, _, Measurements} | Tail], Type, Acc) ->
  NewAcc = extractMeasurements(Measurements, Type, Acc),
  getValuesByType(Tail, Type, NewAcc).

extractMeasurements([], _Type, Acc) -> Acc;
extractMeasurements([{Type, Value} | Tail], Type, Acc) ->
  extractMeasurements(Tail, Type, [Value | Acc]);
extractMeasurements([_ | Tail], Type, Acc) ->
  extractMeasurements(Tail, Type, Acc).

calculateMinAndMax(Readings, Type) ->
  doCalculateMinAndMax(getValuesByType(Readings, Type)).

doCalculateMinAndMax([]) -> {error, no_data_for_type};
doCalculateMinAndMax([FirstValue | Rest]) ->
  findMinMax(Rest, FirstValue, FirstValue).

findMinMax([], Min, Max) ->
  {Min, Max};
findMinMax([Value | Tail], Min, Max) when Value < Min ->
  findMinMax(Tail, Value, Max);
findMinMax([Value | Tail], Min, Max) when Value > Max ->
  findMinMax(Tail, Min, Value);
findMinMax([_Value | Tail], Min, Max) ->
  findMinMax(Tail, Min, Max).

calculateMean(Readings, Type) ->
  doCalculateMean(getValuesByType(Readings, Type)).

doCalculateMean([]) ->
  {error, no_data_for_type};
doCalculateMean(Values) ->
  calcMeanRecursive(Values, 0, 0).

calcMeanRecursive([], Sum, Count) ->
  Sum / Count;
calcMeanRecursive([Value | Tail], Sum, Count) ->
  calcMeanRecursive(Tail, Sum + Value, Count + 1).