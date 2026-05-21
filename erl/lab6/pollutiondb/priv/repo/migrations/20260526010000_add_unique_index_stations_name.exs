defmodule Pollutiondb.Repo.Migrations.AddUniqueIndexStationsName do
  use Ecto.Migration

  def change do
    create unique_index(:stations, [:name], name: :stations_name_index)
  end
end
