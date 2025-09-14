using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;

namespace ICS_Project.BL.Mappers;


public class PlaylistModelMapper : ModelMapperBase<PlaylistEntity, PlaylistListModel, PlaylistDetailModel>
{
    public override PlaylistListModel MapToListModel(PlaylistEntity? entity)
        => entity is null
            ? PlaylistListModel.Empty
            : new PlaylistListModel
            {
                Id = entity.Id,
                Name = entity.Name
            };

    public override PlaylistDetailModel MapToDetailModel(PlaylistEntity? entity)
        => entity is null
            ? PlaylistDetailModel.Empty
            : new PlaylistDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Description = entity.Description
            };

    public PlaylistEntity MapToEntity(PlaylistListModel listModel)
        => new()
        {
            Id = listModel.Id,
            Name = listModel.Name,
            Description = null
        };

    public override PlaylistEntity MapToEntity(PlaylistDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            Name = detailModel.Name,
            Description = detailModel.Description
        };

}