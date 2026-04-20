%%%-------------------------------------------------------------------
%%% @author julia
%%% @copyright (C) 2026, <COMPANY>
%%% @doc
%%% @end
%%%-------------------------------------------------------------------
-module(pollution_gen_server).

-behaviour(gen_server).

-export([start_link/0, stop/0, crash/0]).

-export([addStation/2, addValue/4, removeValue/3, getOneValue/3, getStationMin/2, getStationMean/2, getDailyMean/2, getDailyOverLimit/3, getAirQualityIndex/2]).

-export([init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2, code_change/3]).

-define(SERVER, ?MODULE).

%%%===================================================================
%%% Spawning and gen_server implementation
%%%===================================================================

start_link() ->
    gen_server:start_link({local, ?SERVER}, ?MODULE, [], []).

stop() ->
    gen_server:call(?SERVER, stop).

crash() ->
    gen_server:cast(?SERVER, crash).

%%%===================================================================
%%% API functions (camelCase)
%%%===================================================================

addStation(Name, Coords) ->
    gen_server:call(?SERVER, {add_station, Name, Coords}).

addValue(StationId, Date, Type, Value) ->
    gen_server:call(?SERVER, {add_value, StationId, Date, Type, Value}).

removeValue(StationId, Date, Type) ->
    gen_server:call(?SERVER, {remove_value, StationId, Date, Type}).

getOneValue(StationId, Date, Type) ->
    gen_server:call(?SERVER, {get_one_value, StationId, Date, Type}).

getStationMin(StationId, Type) ->
    gen_server:call(?SERVER, {get_station_min, StationId, Type}).

getStationMean(StationId, Type) ->
    gen_server:call(?SERVER, {get_station_mean, StationId, Type}).

getDailyMean(Type, Date) ->
    gen_server:call(?SERVER, {get_daily_mean, Type, Date}).

getDailyOverLimit(Type, Date, Limit) ->
    gen_server:call(?SERVER, {get_daily_over_limit, Type, Date, Limit}).

getAirQualityIndex(StationId, Date) ->
    gen_server:call(?SERVER, {get_air_quality_index, StationId, Date}).


%%%===================================================================
%%% gen_server callbacks
%%%===================================================================

init([]) ->
    {ok, pollution:create_monitor()}.

handle_call({add_station, Name, Coords}, _From, State) ->
    case pollution:add_station(Name, Coords, State) of
        {error, Reason} -> {reply, {error, Reason}, State};
        NewState -> {reply, ok, NewState}
    end;

handle_call({add_value, StationId, Date, Type, Value}, _From, State) ->
    case pollution:add_value(StationId, Date, Type, Value, State) of
        {error, Reason} -> {reply, {error, Reason}, State};
        NewState -> {reply, ok, NewState}
    end;

handle_call({remove_value, StationId, Date, Type}, _From, State) ->
    case pollution:remove_value(StationId, Date, Type, State) of
        {error, Reason} -> {reply, {error, Reason}, State};
        NewState -> {reply, ok, NewState}
    end;

handle_call({get_one_value, StationId, Date, Type}, _From, State) ->
    Reply = pollution:get_one_value(StationId, Date, Type, State),
    {reply, Reply, State};

handle_call({get_station_min, StationId, Type}, _From, State) ->
    Reply = pollution:get_station_min(StationId, Type, State),
    {reply, Reply, State};

handle_call({get_station_mean, StationId, Type}, _From, State) ->
    Reply = pollution:get_station_mean(StationId, Type, State),
    {reply, Reply, State};

handle_call({get_daily_mean, Type, Date}, _From, State) ->
    Reply = pollution:get_daily_mean(Type, Date, State),
    {reply, Reply, State};

handle_call({get_daily_over_limit, Type, Date, Limit}, _From, State) ->
    Reply = pollution:get_daily_over_limit(Type, Date, Limit, State),
    {reply, Reply, State};

handle_call({get_air_quality_index, StationId, Date}, _From, State) ->
    Reply = pollution:get_air_quality_index(StationId, Date, State),
    {reply, Reply, State};

handle_call(stop, _From, State) ->
    {stop, normal, ok, State};

handle_call(_Request, _From, State) ->
    {reply, {error, unknown_request}, State}.

handle_cast(crash, State) ->
    _ = 1 / 0, %% Intentional error (division by zero)
    {noreply, State};

handle_cast(_Msg, State) ->
    {noreply, State}.

handle_info(_Info, State) ->
    {noreply, State}.

terminate(_Reason, _State) ->
    ok.

code_change(_OldVsn, State, _Extra) ->
    {ok, State}.
