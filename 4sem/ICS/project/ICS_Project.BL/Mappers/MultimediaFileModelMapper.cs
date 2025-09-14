using ICS_Project.DAL.Entities;
using ICS_Project.BL.Models;

namespace ICS_Project.BL.Mappers;

public class MultimediaFileModelMapper:
    ModelMapperBase<MultimediaFileEntity, MultimediaFileListModel, MultimediaFileDetailModel>
{
    public override MultimediaFileListModel MapToListModel(MultimediaFileEntity? entity)
        => entity is null
            ? MultimediaFileListModel.Empty
            : new MultimediaFileListModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Author = entity.Author,
                Url = entity.Url,
            };

    public override MultimediaFileDetailModel MapToDetailModel(MultimediaFileEntity? entity)
        => entity is null
            ? MultimediaFileDetailModel.Empty
            : new MultimediaFileDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Author = entity.Author,
                Url = entity.Url,
            };
    
    public MultimediaFileListModel MapToListModel(MultimediaFileDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            Name = detailModel.Name,
            Author = detailModel.Author,
            Url = detailModel.Url
        };

    public MultimediaFileEntity MapToEntity(MultimediaFileListModel listModel)
        => new()
        {
            Id = listModel.Id,
            Name = listModel.Name,
            Description = listModel.Description,
            Size = listModel.Size,
            Duration = listModel.Duration,
            Author = listModel.Author,
            Url = listModel.Url,
            FileType = listModel.FileType
        };

    public override MultimediaFileEntity MapToEntity(MultimediaFileDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            Name = detailModel.Name,
            Description = detailModel.Description,
            Size = detailModel.Size,
            Duration = detailModel.Duration,
            Author = detailModel.Author,
            Url = detailModel.Url,
            FileType = detailModel.FileType
        };

}