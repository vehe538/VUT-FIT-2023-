using ICS_Project.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.DAL.Seeds;

public static class PlaylistSeeds
{
    public static readonly PlaylistEntity PlaylistEntityOblibene = new()
    {
        Id = Guid.Parse("023024C3-C98B-4B49-A197-6C6873A6060F"),
        Name = "Oblibene",
        Description = "Oblibene pisnicky",
        PictureUrl = "https://site-548611.mozfiles.com/files/548611/srdce-1.png"
    };


    static PlaylistSeeds()
    {
        PlaylistEntityOblibene.MultimediaFiles.Add(TimeAddedSeeds.OblibeneKabati);
        PlaylistEntityOblibene.MultimediaFiles.Add(TimeAddedSeeds.OblibeneKarelGott);


        PlaylistEntityOblibene.TotalDuration = 0;
        foreach (var file in PlaylistEntityOblibene.MultimediaFiles)
        {
            PlaylistEntityOblibene.TotalDuration += file.MultimediaFile!.Duration;
        }
        PlaylistEntityOblibene.FileCount = PlaylistEntityOblibene.MultimediaFiles.Count();
    }

    public static DbContext SeedPlaylists(this DbContext dbx)
    {
        dbx.Set<PlaylistEntity>().AddRange(
            PlaylistEntityOblibene with { MultimediaFiles = new List<TimeAddedEntity>()}
        );
        return dbx;
    }

}