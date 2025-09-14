using System.Collections.ObjectModel;

namespace ICS_Project.BL.Models;

public record PlaylistDetailModel : ModelBase
{
    public required string Name { get; set; }
    public int FileCount { get; set; }
    public int TotalDuration { get; set; }
    public required string Description { get; set; }
    public string? PictureUrl { get; set; }
    public ObservableCollection<TimeAddedListModel> MultimediaFiles { get; init; } = new();

    public static PlaylistDetailModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        Description = string.Empty
    };
}