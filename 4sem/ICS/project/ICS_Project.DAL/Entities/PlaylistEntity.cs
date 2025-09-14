namespace ICS_Project.DAL.Entities;

public record PlaylistEntity : IEntity
{
    public required Guid Id { get; set; }
    public required string Name { get; set; }
    public int FileCount { get; set; }
    public int TotalDuration { get; set; }
    public required string Description { get; set; }

    public string? PictureUrl { get; set; }

    public ICollection<TimeAddedEntity> MultimediaFiles { get; init; } = new List<TimeAddedEntity>();


}