namespace ICS_Project.App.Messages;

public record IngredientEditMessage
{
    public required Guid MultimediaFileId { get; set; }
}