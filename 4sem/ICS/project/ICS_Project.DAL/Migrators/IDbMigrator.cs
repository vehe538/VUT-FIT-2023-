namespace ICS_Project.DAL.Migrators;
public interface IDbMigrator
{
    public void Migrate();
    public Task MigrateAsync(CancellationToken cancellationToken);
}
