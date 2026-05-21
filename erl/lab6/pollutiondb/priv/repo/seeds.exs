alias Pollutiondb.{PollutionDataLoader, Reading, Repo, Station}

csv_path = Path.expand("../data/AirlyData-ALL-50k.csv", __DIR__)
lines = PollutionDataLoader.load_file(csv_path)

Repo.delete_all(Reading)
Repo.delete_all(Station)

station_rows = PollutionDataLoader.unique_stations(lines)

stations_by_key =
  Enum.reduce(station_rows, %{}, fn station_row, acc ->
    case Station.add(station_row.station_name, station_row.lon, station_row.lat) do
      {:ok, station} -> Map.put(acc, station_row.station_id, station)
      {:error, _changeset} ->
        station = Repo.get_by(Station, name: station_row.station_name) ||
                    raise("Failed to insert or find station #{station_row.station_name}")
        Map.put(acc, station_row.station_id, station)
    end
  end)

PollutionDataLoader.parse_lines(lines)
|> Enum.each(fn reading_row ->
  station = Map.fetch!(stations_by_key, reading_row.station_id)
  case Reading.add(station, reading_row.date, reading_row.time, reading_row.type, reading_row.value) do
    {:ok, _reading} -> :ok
    {:error, _changeset} -> :ok
  end
end)

Enum.zip(
  Enum.take(station_rows, 3),
  [{"PM10", 42.0}, {"PM25", 27.5}, {"TEMPERATURE", 8.1}]
)
|> Enum.each(fn {station_row, {type, value}} ->
  station = Map.fetch!(stations_by_key, station_row.station_id)
  case Reading.add_now(station, type, value) do
    {:ok, _} -> :ok
    {:error, _} -> :ok
  end
end)

today_count = Reading.find_by_date(Date.utc_today()) |> length()

IO.puts("Imported #{map_size(stations_by_key)} stations and #{length(lines)} readings")
IO.puts("Added #{today_count} readings for today")
