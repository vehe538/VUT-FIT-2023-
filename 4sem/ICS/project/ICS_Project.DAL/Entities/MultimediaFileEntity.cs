using ICS_Project.Common.Enums;

namespace ICS_Project.DAL.Entities
{
    public record MultimediaFileEntity : IEntity
    {
        public required Guid Id { get; set; }
        public required string Name { get; set; }
        public int Size { get; set; }
        public int Duration { get; set; }
        public required string Author { get; set; }
        public required string Url { get; set; }
        public FileType FileType { get; set; }
        public string? Description { get; set; }
        public string? PictureUrl { get; set; }
    }
}
