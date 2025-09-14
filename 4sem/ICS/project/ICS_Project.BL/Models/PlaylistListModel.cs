namespace ICS_Project.BL.Models;

public record PlaylistListModel : ModelBase
{
    public required string Name { get; set; }
    public int FileCount { get; set; }
    public string? PictureUrl { get; set; }

    public static PlaylistListModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
    };
}