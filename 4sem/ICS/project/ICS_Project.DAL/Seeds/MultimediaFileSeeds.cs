using ICS_Project.DAL.Entities;
using ICS_Project.Common.Enums;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.DAL.Seeds;

public static class MultimediaFileSeeds
{
    public static readonly MultimediaFileEntity Kabati = new()
    {
        Id = Guid.Parse("1D283B65-6398-49DD-B58B-6DBAFAFD513F"),
        Name = "Corrida",
        Size = 100,
        Duration = 172,
        Author = "Kabat",
        Url = "https://youtu.be/lz3MA-oU-84?si=TNBzbOEie6JMh579",
        FileType = FileType.Music,
        Description = "Corrida od Kabatu",
        PictureUrl = "https://data.bontonland.cz/fotky/581/p-231197-full.jpg"
    };

    public static readonly MultimediaFileEntity KarelGott = new()
    {
        Id = Guid.Parse("B86E8C19-3147-48D3-90DC-3629B343A5AE"),
        Name = "Trezor",
        Size = 100,
        Duration = 142,
        Author = "Karel Gott",
        Url = "https://youtu.be/Mcf_dekZPzk?si=p1aqu1NJpXJI-fxE",
        FileType = FileType.Music,
        Description = "Hitovka Trezor od Kaji",
        PictureUrl = "https://i.scdn.co/image/ab67616d0000b273733846332470688021b03be9"
    };

    
    
    public static readonly MultimediaFileEntity JanGott = new()
    {
        Id = Guid.Parse("B86E8C19-3147-48D3-90DC-3629B343A5BE"),
        Name = "asd",
        Size = 100,
        Duration = 142,
        Author = "Karel Gott",
        Url = "https://youtu.be/Mcf_dekZPzk?si=p1aqu1NJpXJI-fxE",
        FileType = FileType.Video,
        Description = "Hitovka Trezor od Kaji",
        PictureUrl = "https://i.scdn.co/image/ab67616d0000b273733846332470688021b03be9"
    };
    
    public static readonly MultimediaFileEntity FilipGott = new()
    {
        Id = Guid.Parse("B86E8C19-3147-48D3-90DC-3629B343A5CE"),
        Name = "Hitovka Trezor",
        Size = 100,
        Duration = 142,
        Author = "Karel Gott",
        Url = "https://youtu.be/Mcf_dekZPzk?si=p1aqu1NJpXJI-fxE",
        FileType = FileType.Music, 
        Description = "Hitovka Trezor od Kabatu",
        PictureUrl = "https://i.scdn.co/image/ab67616d0000b273733846332470688021b03be9"
    };
    
    public static readonly MultimediaFileEntity Empty = new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        Size = 0,
        Duration = 0,
        Author = string.Empty,
        Url = string.Empty,
        FileType = FileType.Music, 
        Description = string.Empty,
        PictureUrl = string.Empty
    };

    
    public static DbContext SeedMultimediaFiles(this DbContext dbx)
    {
        dbx.Set<MultimediaFileEntity>().AddRange(
            Kabati,
            KarelGott,
            FilipGott,
            JanGott,
            Empty
                
        );

        return dbx;
    }

}





