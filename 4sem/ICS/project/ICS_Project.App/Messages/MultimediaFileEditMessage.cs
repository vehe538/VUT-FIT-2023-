namespace ICS_Project.App.Messages;

public record MultimediaFileEditMessage
{
    public required Guid MultimediaFileId { get; init; }
}