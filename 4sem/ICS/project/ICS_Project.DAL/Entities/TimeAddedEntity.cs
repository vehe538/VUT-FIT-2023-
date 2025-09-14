namespace ICS_Project.DAL.Entities;

public record TimeAddedEntity : IEntity
{
    public required Guid Id { get; set; }
    public DateTime DateTime {get; set; }

    public required Guid MultimediaId { get; set; }
    public required Guid PlaylistId { get; set; }

    public required PlaylistEntity? Playlist { get; init; }
    public required MultimediaFileEntity? MultimediaFile { get; init; }

}