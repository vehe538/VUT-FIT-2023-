
namespace ICS_Project.DAL.Options;

public record DALOptions
{
    public required string DatabaseDirectory { get; init; }
    public string DatabaseName { get; init; } = null!;
    public string DatabaseFilePath => Path.Combine(DatabaseDirectory, DatabaseName!);

    /// Deletes database before application startup
    public bool RecreateDatabaseEachTime { get; init; } = false;

    /// Seeds DemoData from DbContext on database creation.
    public bool SeedDemoData { get; init; } = false;

    /// Recreates database on seed to ensure seed data consistency.
    public bool RecreateOnSeed { get; init; } = false;
}

