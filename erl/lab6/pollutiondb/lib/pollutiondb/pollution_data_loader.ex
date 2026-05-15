defmodule Pollutiondb.PollutionDataLoader do
  def load_file(file_path) do
    file_path
    |> File.read!()
    |> String.trim()
    |> String.split("\n", trim: true)
  end

  def parse_line(line) do
    [datetime, type, value, station_id, station_name, location] = String.split(line, ";")
    [lat, lon] = String.split(location, ",") |> Enum.map(&(elem(Float.parse(&1), 0)))
    {:ok, date_time} = NaiveDateTime.from_iso8601(datetime)

    %{
      station_id: String.to_integer(station_id),
      station_name: station_name,
      lat: lat,
      lon: lon,
      date: NaiveDateTime.to_date(date_time),
      time: NaiveDateTime.to_time(date_time) |> Time.truncate(:second),
      type: type,
      value: elem(Float.parse(value), 0)
    }
  end

  def parse_lines(lines) do
    Enum.map(lines, &parse_line/1)
  end

  def unique_stations(lines) do
    lines
    |> parse_lines()
    |> Enum.uniq_by(& &1.station_id)
    |> Enum.map(fn station ->
      %{
        station_id: station.station_id,
        station_name: station.station_name,
        lon: station.lon,
        lat: station.lat
      }
    end)
  end
end
