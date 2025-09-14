using ICS_Project.DAL.Entities;

namespace ICS_Project.DAL.Mappers;

public class TimeAddedEntityMapper : IEntityMapper<TimeAddedEntity>
{
    public void MapToExistingEntity(TimeAddedEntity existingEntity, TimeAddedEntity newEntity)
    {
        existingEntity.DateTime = newEntity.DateTime;
        existingEntity.MultimediaId = newEntity.MultimediaId;
        existingEntity.PlaylistId = newEntity.PlaylistId;

    }
}