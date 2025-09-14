using ICS_Project.Common.Enums;

namespace ICS_Project.BL.Models;

public record MultimediaFileListModel : ModelBase
{
    public required string Name { get; set; }
    public int Size { get; set; }
    public int Duration { get; set; }
    public required string Author { get; set; }
    public required string Url { get; set; }
    public FileType FileType { get; set; }
    public string? Description { get; set; }

    public string? PictureUrl { get; set; }

    public static MultimediaFileListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        Name = string.Empty,
        Author = string.Empty,
        Url = string.Empty
    };
}