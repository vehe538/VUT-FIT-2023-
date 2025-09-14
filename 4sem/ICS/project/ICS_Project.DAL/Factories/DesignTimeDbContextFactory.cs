using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace ICS_Project.DAL.Factories
{
    /// <summary>
    ///     EF Core CLI migration generation uses this DbContext to create model and migration
    /// </summary>
    public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<MediaPlayerDbContext>
    {
        private readonly DbContextSqLiteFactory _dbContextSqLiteFactory = new("mediaplayer.db");

        public MediaPlayerDbContext CreateDbContext(string[] args) => _dbContextSqLiteFactory.CreateDbContext();
    }
}

