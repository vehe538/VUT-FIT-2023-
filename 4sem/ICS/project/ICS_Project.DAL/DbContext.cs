
using ICS_Project.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.DAL
{
    public class MediaPlayerDbContext(DbContextOptions contextOptions) : DbContext(contextOptions)
    {

        public DbSet<MultimediaFileEntity> MultimediaFiles { get; set; }
        public DbSet<PlaylistEntity> Playlists { get; set; }
        public DbSet<TimeAddedEntity> TimesAdded { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Entity<PlaylistEntity>()
                .HasMany(i => i.MultimediaFiles)
                .WithOne(i => i.Playlist)
                .OnDelete(DeleteBehavior.Cascade);

            modelBuilder.Entity<MultimediaFileEntity>()
                .HasMany<TimeAddedEntity>()
                .WithOne(i => i.MultimediaFile)
                .OnDelete(DeleteBehavior.Restrict);

        }

    }
}
