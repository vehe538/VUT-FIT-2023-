using ICS_Project.DAL.Options;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

namespace ICS_Project.DAL.Seeds;

public class DbSeeder(IDbContextFactory<MediaPlayerDbContext> dbContextFactory, DALOptions options) : IDbSeeder
{
    public void Seed() => SeedAsync(CancellationToken.None).GetAwaiter().GetResult();

    public async Task SeedAsync(CancellationToken cancellationToken)
    {
        await using MediaPlayerDbContext dbContext = await dbContextFactory.CreateDbContextAsync(cancellationToken);


        if (options.SeedDemoData)
        {
            if (options.RecreateOnSeed)
            {
                dbContext.Database.EnsureDeleted();
                dbContext.Database.EnsureCreated();
            }

            dbContext
                .SeedMultimediaFiles()
                .SeedPlaylists()
                .SeedTimeAdded();

            await dbContext.SaveChangesAsync(cancellationToken);
            
        }
    }
}