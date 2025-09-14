using ICS_Project.DAL.Entities;

namespace ICS_Project.DAL.Mappers;

public class MultimediaFileEntityMapper : IEntityMapper<MultimediaFileEntity>
{
    public void MapToExistingEntity(MultimediaFileEntity existingEntity, MultimediaFileEntity newEntity)
    {
        existingEntity.Name = newEntity.Name;
        existingEntity.Size = newEntity.Size;
        existingEntity.Duration = newEntity.Duration;
        existingEntity.Author = newEntity.Author;
        existingEntity.Url = newEntity.Url;
        existingEntity.FileType = newEntity.FileType;
        existingEntity.Description = newEntity.Description;
        existingEntity.PictureUrl = newEntity.PictureUrl;
    }
}