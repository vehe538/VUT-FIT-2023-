using ICS_Project.DAL.Entities;

namespace ICS_Project.DAL.Mappers;

public class PlaylistEntityMapper : IEntityMapper<PlaylistEntity>
{
    public void MapToExistingEntity(PlaylistEntity existingEntity, PlaylistEntity newEntity)
    {
        //Maybe
        //existingEntity.FileCount = newEntity.MultimediaFiles.Count();

        existingEntity.FileCount = newEntity.FileCount;
        existingEntity.TotalDuration = newEntity.TotalDuration;


        existingEntity.Name = newEntity.Name;
        existingEntity.Description = newEntity.Description;
        existingEntity.PictureUrl = newEntity.PictureUrl;
    }
}