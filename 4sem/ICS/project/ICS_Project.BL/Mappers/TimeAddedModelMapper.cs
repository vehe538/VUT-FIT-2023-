using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;

namespace ICS_Project.BL.Mappers;


public class TimeAddedModelMapper : ModelMapperBase<TimeAddedEntity, TimeAddedListModel, TimeAddedDetailModel>
{
    public override TimeAddedListModel MapToListModel(TimeAddedEntity? entity)
        => entity is null
            ? TimeAddedListModel.Empty
            : new TimeAddedListModel
            {
                Id = entity.Id,
                MultimediaId = entity.MultimediaId,
                MultimediaName = string.Empty,
                MultimediaAuthor = string.Empty,
                MultimediaUrl = string.Empty
            };

    public override TimeAddedDetailModel MapToDetailModel(TimeAddedEntity? entity)
        => entity is null
            ? TimeAddedDetailModel.Empty
            : new TimeAddedDetailModel
            {
                Id = entity.Id,
                MultimediaId = entity.MultimediaId,
                MultimediaName = string.Empty,
                MultimediaAuthor = string.Empty,
                MultimediaUrl = string.Empty
            };

    public TimeAddedListModel MapToListModel(TimeAddedDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            MultimediaId = detailModel.MultimediaId,
            MultimediaName = detailModel.MultimediaName,
            MultimediaAuthor = detailModel.MultimediaAuthor,
            MultimediaUrl = detailModel.MultimediaUrl,
            MultimediaDescription = detailModel.MultimediaDescription,
            MultimediaDuration = detailModel.MultimediaDuration,
            MultimediaSize = detailModel.MultimediaSize,
            MultimediaFileType = detailModel.MultimediaFileType,
        };

    public TimeAddedEntity MapToEntity(TimeAddedListModel listModel, Guid playlistId)
        => new()
        {
            Id = listModel.Id,
            MultimediaId = listModel.MultimediaId,
            PlaylistId = playlistId,
            Playlist = null,
            MultimediaFile = null
        };

    public TimeAddedEntity MapToEntity(TimeAddedDetailModel detailModel, Guid playlistId)
        => new()
        {
            Id = detailModel.Id,
            MultimediaId = detailModel.MultimediaId,
            PlaylistId = playlistId,
            Playlist = null,
            MultimediaFile = null
        };

    public void MapToExistingDetailModel(TimeAddedDetailModel existingDetailModel, 
        MultimediaFileListModel multimediaFile)
    {
        existingDetailModel.MultimediaId = multimediaFile.Id;
        existingDetailModel.MultimediaName = multimediaFile.Name;
        existingDetailModel.MultimediaUrl = multimediaFile.Url;

    }

    public override TimeAddedEntity MapToEntity(TimeAddedDetailModel model)
        => throw new NotImplementedException("This method is unsupported. Use the other overload.");
}