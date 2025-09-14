using ICS_Project.Common.Enums;

namespace ICS_Project.BL.Models;

public record TimeAddedDetailModel : ModelBase
{
    public required Guid MultimediaId { get; set; }
    public required string MultimediaName { get; set; }
    public int MultimediaSize { get; set; }
    public int MultimediaDuration { get; set; }
    public required string MultimediaAuthor { get; set; }
    public required string MultimediaUrl { get; set; }
    public FileType MultimediaFileType { get; set; }
    public string? MultimediaDescription { get; set; }
    public string? MultimediaPictureUrl { get; set; }
    public DateTime DateTime { get; set; }

    public static TimeAddedDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        MultimediaId = Guid.Empty,
        MultimediaName = string.Empty,
        MultimediaAuthor = string.Empty,
        MultimediaUrl = string.Empty
    };
}