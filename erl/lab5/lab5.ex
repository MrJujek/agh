# Run as: iex --dot-iex path/to/notebook.exs

# Title: lab5

Code.append_path("c:/Users/julia/github/agh/erl/lab4/_build/default/lib/lab4/ebin")
Application.start(:lab4)

defmodule PollutionDataLoader do
  def load_file(file_path) do
    File.read!(file_path)
    |> String.trim()
    |> String.split("\n")
  end

  def parse_line(line) do
    [datetime, type, val, id, name, location] = line
                                              |> String.split(";")
    %{
      :stationId => id
        |> Integer.parse()
        |> elem(0),
      :datetime => datetime
              |> NaiveDateTime.from_iso8601!()
              |> NaiveDateTime.to_erl(),
      :pollutionLevel => val
          |> Float.parse()
          |> elem(0),
      :stationName => name,
      :location => location
              |> String.split(",")
              |> Enum.map(&Float.parse/1) # fn(a)->String.to_float(a) end
              |> Enum.map(& elem(&1, 0))
              |> :erlang.list_to_tuple,
      :pollutionType => type,
      :stationKey => "#{id} #{name}"
    }
  end

  def parse_lines(lines) do
    Enum.map(lines, &PollutionDataLoader.parse_line/1)
  end

  def load_stations(lines) do
    lines
    |> Enum.map(&PollutionDataLoader.parse_line/1)
    |> Enum.uniq_by(& &1.stationId)
    |> Enum.map(fn station ->
      %{
        stationId: station.stationId,
        stationName: "#{station.stationId} #{station.stationName}",
        location: station.location
      }
    end)
  end

  def identify_stations(lines) do
    lines
    |> Enum.map(fn(a) -> PollutionDataLoader.parse_line(a) end)
    |> Enum.uniq_by(& &1.stationId)
    |> length
  end
end

data = PollutionDataLoader.load_file("c:/Users/julia/AppData/Local/livebook/autosaved/2026_04_28/09_40_pcta/files/AirlyData-ALL-50k.csv")

lines = data

length(lines)

PollutionDataLoader.parse_line("2024-02-10T09:00:00.000Z;PM1;17.07;57570;Polska, Kraków, Floriana Straszewskiego;50.057224,19.933157")

PollutionDataLoader.identify_stations(lines)

stations = PollutionDataLoader.load_stations(lines)

length(stations)

monitor = :pollution.create_monitor()

{load_stations_time_us, monitor_after_stations} =
  :timer.tc(fn ->
    Enum.reduce(stations, monitor, fn station, acc ->
      :pollution.add_station(String.to_charlist(station.stationName), station.location, acc)
    end)
  end)

load_stations_time_s = load_stations_time_us / 1_000_000

{load_measurements_time_us, measurements} =
  :timer.tc(fn -> PollutionDataLoader.parse_lines(lines) end)

load_measurements_time_s = load_measurements_time_us / 1_000_000

{load_measurements_into_monitor_time_us, monitor_after_measurements} =
  :timer.tc(fn ->
    Enum.reduce(measurements, monitor_after_stations, fn measurement, acc ->
      :pollution.add_value(
        String.to_charlist("#{measurement.stationId} #{measurement.stationName}"),
        measurement.datetime,
        String.to_charlist(measurement.pollutionType),
        measurement.pollutionLevel,
        acc
      )
    end)
  end)

load_measurements_into_monitor_time_s = load_measurements_into_monitor_time_us / 1_000_000

{get_station_min_time_us, station_min_pm10} =
  :timer.tc(fn ->
    :pollution.get_station_min(String.to_charlist("9910 Polska, Kraków, Studencka"), String.to_charlist("PM10"), monitor_after_measurements)
  end)

get_station_min_time_s = get_station_min_time_us / 1_000_000

{get_daily_mean_time_us, daily_mean_pm25} =
  :timer.tc(fn ->
    :pollution.get_daily_mean(String.to_charlist("PM25"), {2024, 2, 10}, monitor_after_measurements)
  end)

get_daily_mean_time_s = get_daily_mean_time_us / 1_000_000
