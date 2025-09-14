namespace ICS_Project.App.Messages;

public record PlaylistEditMessage
{
    public required Guid PlaylistId { get; set; }
}