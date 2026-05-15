defmodule Pollutiondb.Reading do
  use Ecto.Schema
  require Ecto.Query

  schema "readings" do
    field :date, :date
    field :time, :time
    field :type, :string
    field :value, :float

    belongs_to :station, Pollutiondb.Station
  end

  def add(station, date, time, type, value) do
    Ecto.build_assoc(station, :readings)
    |> changeset(%{
      date: date,
      time: time,
      type: type,
      value: value
    })
    |> Pollutiondb.Repo.insert()
  end

  def add_now(station, type, value) do
    add(station, Date.utc_today(), Time.utc_now() |> Time.truncate(:second), type, value)
  end

  def find_by_date(date) do
    Ecto.Query.from(r in Pollutiondb.Reading, where: r.date == ^date)
    |> Pollutiondb.Repo.all()
  end

  defp changeset(reading, changes_map) do
    Ecto.Changeset.cast(reading, changes_map, [:date, :time, :type, :value])
    |> Ecto.Changeset.validate_required([:date, :time, :type, :value])
  end
end
