defmodule Pollutiondb.Repo.Migrations.AddUniqueIndexReadingsStationDateTimeType do
  use Ecto.Migration

  def change do
    create unique_index(:readings, [:station_id, :date, :time, :type], name: :readings_station_date_time_type_index)
  end
end
