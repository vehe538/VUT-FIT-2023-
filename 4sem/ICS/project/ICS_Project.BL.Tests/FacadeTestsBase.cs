using ICS_Project.BL.Mappers;
using ICS_Project.DAL;
using ICS_Project.DAL.Factories;
using ICS_Project.DAL.Seeds;
using ICS_Project.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;
using Xunit.Abstractions;

namespace ICS_Project.BL.Tests;

public class FacadeTestsBase : IAsyncLifetime
{
    protected FacadeTestsBase(ITestOutputHelper output)
    {
        XUnitTestOutputConverter converter = new(output);
        Console.SetOut(converter);

        DbContextFactory = new DbContextSqLiteFactory(GetType().FullName!);

        MultimediaFileModelMapper = new MultimediaFileModelMapper();
        PlaylistModelMapper = new PlaylistModelMapper();

        UnitOfWorkFactory = new UnitOfWorkFactory(DbContextFactory);
    }

    protected IDbContextFactory<MediaPlayerDbContext> DbContextFactory { get; }

    protected MultimediaFileModelMapper MultimediaFileModelMapper { get; }
    protected PlaylistModelMapper PlaylistModelMapper { get; }
    protected UnitOfWorkFactory UnitOfWorkFactory { get; }

    public async Task InitializeAsync()
    {
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        await dbx.Database.EnsureDeletedAsync();
        await dbx.Database.EnsureCreatedAsync();

        dbx.SeedMultimediaFiles();
        await dbx.SaveChangesAsync();
    }

    public async Task DisposeAsync()
    {
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        await dbx.Database.EnsureDeletedAsync();
    }
}