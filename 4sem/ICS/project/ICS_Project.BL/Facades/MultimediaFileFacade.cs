using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;
using ICS_Project.DAL.Mappers;
using ICS_Project.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.BL.Facades;

public class MultimediaFileFacade(
    IUnitOfWorkFactory unitOfWorkFactory,
    MultimediaFileModelMapper modelMapper)
    :
        FacadeBase<MultimediaFileEntity, MultimediaFileListModel, MultimediaFileDetailModel, MultimediaFileEntityMapper>(
            unitOfWorkFactory, modelMapper), IMultimediaFileFacade
{
    public async Task<IEnumerable<MultimediaFileListModel>> GetFilteredAsync(MultimediaFileFilter filter)
    {
        await using var uow = UnitOfWorkFactory.Create();
        IQueryable<MultimediaFileEntity> query = uow.GetRepository<MultimediaFileEntity, MultimediaFileEntityMapper>().Get();

        if (!string.IsNullOrEmpty(filter.Search))
        {
            query = query.Where(m => m.Name.Contains(filter.Search));
        }

        if (filter.FileType.HasValue)
        {
            query = query.Where(m => m.FileType == filter.FileType);
        }

        if (filter.MinDuration.HasValue)
        {
            query = query.Where(m => m.Duration >= filter.MinDuration.Value);
        }

        if (filter.MaxDuration.HasValue)
        {
            query = query.Where(m => m.Duration <= filter.MaxDuration.Value);
        }

        query = filter.SortBy?.ToLower() switch
        {
            "duration" => filter.Descending
                ? query.OrderByDescending(m => m.Duration)
                : query.OrderBy(m => m.Duration),
            "title" => filter.Descending
                ? query.OrderByDescending(m => m.Name)
                : query.OrderBy(m => m.Name),
            _ => query.OrderBy(m => m.Name)
        };

        var entities = await query.ToListAsync();
        return ModelMapper.MapToListModel(entities);
    }
}
