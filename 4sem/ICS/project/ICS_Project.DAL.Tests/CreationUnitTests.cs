using System.Diagnostics;
using ICS_Project.DAL.Entities;
using ICS_Project.Common.Enums;
using ICS_Project.DAL.Factories;
using Microsoft.EntityFrameworkCore;
using ICS_Project.DAL.Seeds;

namespace ICS_Project.DAL.Tests
{
    public class CreationUnitTest : IDisposable
    {
        private readonly MediaPlayerDbContext _dbx;

        public CreationUnitTest()
        {
            var dbContextFactory = new DbContextSqLiteFactory("testing");
            _dbx = dbContextFactory.CreateDbContext();
            _dbx.Database.EnsureDeleted();
            _dbx.Database.EnsureCreated();
        }


        [Fact]
        public void Test_Seeds()
        {
            // arrange
            _dbx
                .SeedMultimediaFiles()
                .SeedPlaylists()
                .SeedTimeAdded();

            _dbx.SaveChanges();

            // act
            var retrievedPlaylist = _dbx.Playlists
                .Include(playlist => playlist.MultimediaFiles)
                .ThenInclude(timeAdded => timeAdded.MultimediaFile)
                .FirstOrDefault(p => p.Name == "Oblibene");

            // assert
            Assert.NotNull(retrievedPlaylist);
            Assert.Equal("Oblibene", retrievedPlaylist.Name);
            Assert.Equal(2, retrievedPlaylist.MultimediaFiles.Count);

            foreach (var time in retrievedPlaylist.MultimediaFiles)
            {
                Debug.Assert(time.MultimediaFile != null, "time.MultimediaFileEntity != null");
                switch (time.MultimediaFile.Name)
                {
                    case "Corrida":
                        Assert.Equal("Corrida", time.MultimediaFile.Name);
                        Assert.Equal("Kabat", time.MultimediaFile.Author);
                        Assert.Equal(172, time.MultimediaFile.Duration);
                        Assert.Equal("https://youtu.be/lz3MA-oU-84?si=TNBzbOEie6JMh579", time.MultimediaFile.Url);
                        Assert.Equal(FileType.Music, time.MultimediaFile.FileType);
                        Assert.Equal("Corrida od Kabatu", time.MultimediaFile.Description);
                        break;
                    case "Trezor":
                        Assert.Equal("Trezor", time.MultimediaFile.Name);
                        Assert.Equal("Karel Gott", time.MultimediaFile.Author);
                        Assert.Equal(142, time.MultimediaFile.Duration);
                        Assert.Equal("https://youtu.be/Mcf_dekZPzk?si=p1aqu1NJpXJI-fxE", time.MultimediaFile.Url);
                        Assert.Equal(FileType.Music, time.MultimediaFile.FileType);
                        Assert.Equal("Hitovka Trezor od Kaji", time.MultimediaFile.Description);
                        break;
                    default:
                        throw new Exception("Unexpected multimedia file in playlist");
                }
            }

        }

        [Fact]
        public void Test_Creation_UnitTest_Constructor()
        {
            Assert.NotNull(_dbx);

            // check if the database is empty
            var retrieve = _dbx.MultimediaFiles.FirstOrDefault();
            Assert.Null(retrieve);

        }

        [Fact]
        public void Test_MultiMediaFile_Creation()
        {
            
            var mmfile = new MultimediaFileEntity()
            {
                Id = Guid.NewGuid(),
                Name = "test",
                Url = "test",
                Duration = 100,
                Size = 100,
                Author = "test",
                FileType = FileType.Music,
                Description = "test",
                PictureUrl = "test"

            };

            _dbx.MultimediaFiles.Add(mmfile);
            _dbx.SaveChanges();

            var retrieved = _dbx.MultimediaFiles.Find(mmfile.Id);
            Assert.NotNull(retrieved);
            Assert.Equal("test", retrieved.Name);
            Assert.Equal("test", retrieved.Url);
        }

        [Fact]
        public void Test_Playlist_Creation()
        {

            var playlist = new PlaylistEntity()
            {
                Id = Guid.NewGuid(),
                Name = "My PlaylistEntity",
                FileCount = 5,
                TotalDuration = 300,
                Description = "A test playlist",
                MultimediaFiles = new List<TimeAddedEntity>()
            };

            _dbx.Playlists.Add(playlist);
            _dbx.SaveChanges();

            var retrievedPlaylist = _dbx.Playlists.Find(playlist.Id);

            Assert.NotNull(retrievedPlaylist);
            Assert.Equal("My PlaylistEntity", retrievedPlaylist.Name);
            Assert.Equal(5, retrievedPlaylist.FileCount);
            Assert.Equal(300, retrievedPlaylist.TotalDuration);
            Assert.Equal("A test playlist", retrievedPlaylist.Description);
            Assert.Empty(retrievedPlaylist.MultimediaFiles);
        }

        [Fact]
        public void Test_Playlist_With_MultimediaFiles()
        {

            var multimediaFile = new MultimediaFileEntity()
            {
                Id = Guid.NewGuid(),
                Name = "Test Song",
                Url = "test_url",
                Duration = 200,
                Size = 5000,
                Author = "Test Artist",
                FileType = FileType.Music,
                Description = "Test Description",
                PictureUrl = "test_picture"
            };

            var playlist = new PlaylistEntity()
            {
                Id = Guid.NewGuid(),
                Name = "My PlaylistEntity",
                FileCount = 5,
                TotalDuration = 300,
                Description = "A test playlist",
                MultimediaFiles = new List<TimeAddedEntity>()
            };

            var timeAdded = new TimeAddedEntity()
            {
                Id = Guid.NewGuid(),
                DateTime = DateTime.UtcNow,
                MultimediaId = multimediaFile.Id,
                PlaylistId = playlist.Id,
                MultimediaFile = multimediaFile,
                Playlist = playlist
            };

            _dbx.MultimediaFiles.Add(multimediaFile);
            _dbx.Playlists.Add(playlist);
            _dbx.TimesAdded.Add(timeAdded);
            _dbx.SaveChanges();

            // Retrieve from DB
            var retrievedPlaylist = _dbx.Playlists
                .Include(p => p.MultimediaFiles)
                .ThenInclude(t => t.MultimediaFile)
                .FirstOrDefault(p => p.Id == playlist.Id);

            Assert.NotNull(retrievedPlaylist);
            Assert.Equal("My PlaylistEntity", retrievedPlaylist.Name);
            Assert.Single(retrievedPlaylist.MultimediaFiles);

            var retrievedTimeAdded = retrievedPlaylist.MultimediaFiles.First();
            Assert.Equal(multimediaFile.Id, retrievedTimeAdded.MultimediaId);
            Assert.Equal(playlist.Id, retrievedTimeAdded.PlaylistId);
            Assert.NotNull(retrievedTimeAdded.MultimediaFile);
            Assert.Equal("Test Song", retrievedTimeAdded.MultimediaFile.Name);
        }

        public void Dispose()
        {
            _dbx?.Dispose();
        }
    }
}