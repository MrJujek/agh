defmodule Pollutiondb.Repo.Migrations.DedupeStationsByName do
  use Ecto.Migration

  def up do
    # remove duplicate stations by name, keep the one with smallest id
    execute("""
    DELETE FROM stations
    WHERE id NOT IN (SELECT MIN(id) FROM stations GROUP BY name);
    """)
  end

  def down do
    # irreversible
    :ok
  end
end
