namespace ICS_Project.DAL.Seeds;

public interface IDbSeeder
{
    void Seed();
    Task SeedAsync(CancellationToken cancellationToken);
}