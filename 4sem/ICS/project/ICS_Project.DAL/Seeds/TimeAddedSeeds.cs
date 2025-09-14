using ICS_Project.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.DAL.Seeds;

public static class TimeAddedSeeds
{
    public static readonly TimeAddedEntity OblibeneKabati = new()
    {
        Id = Guid.Parse("FF7F9138-85DC-4628-A65D-064868258CC8"),
        DateTime = DateTime.Now,
        MultimediaId = MultimediaFileSeeds.Kabati.Id,
        PlaylistId = PlaylistSeeds.PlaylistEntityOblibene.Id,
        MultimediaFile = MultimediaFileSeeds.Kabati,
        Playlist = PlaylistSeeds.PlaylistEntityOblibene
    };

    public static readonly TimeAddedEntity OblibeneKarelGott = new()
    {
        Id = Guid.Parse("A2714C66-3A47-457B-B905-B00D0437D100"),
        DateTime = DateTime.Now,
        MultimediaId = MultimediaFileSeeds.KarelGott.Id,
        PlaylistId = PlaylistSeeds.PlaylistEntityOblibene.Id,
        MultimediaFile = MultimediaFileSeeds.KarelGott,
        Playlist = PlaylistSeeds.PlaylistEntityOblibene
    };

    public static DbContext SeedTimeAdded(this DbContext dbx)
    {
        dbx.Set<TimeAddedEntity>().AddRange(
            OblibeneKabati with{ Playlist = null!},
            OblibeneKarelGott with{ Playlist = null!}
        );
        return dbx;
    }
}