using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;
using ICS_Project.DAL.Mappers;
using ICS_Project.DAL.Repositories;
using ICS_Project.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.BL.Facades;
public class TimeAddedFacade(
    IUnitOfWorkFactory unitOfWorkFactory,
    TimeAddedModelMapper timeAddedModelMapper)
    :
        FacadeBase<TimeAddedEntity, TimeAddedListModel, TimeAddedDetailModel,
            TimeAddedEntityMapper>(unitOfWorkFactory, timeAddedModelMapper), ITimeAddedFacade
{
    public async Task SaveAsync(TimeAddedListModel model, Guid playlistId)
    {
        TimeAddedEntity entity = timeAddedModelMapper.MapToEntity(model, playlistId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<TimeAddedEntity> repository =
            uow.GetRepository<TimeAddedEntity, TimeAddedEntityMapper>();

        if (await repository.ExistsAsync(entity))
        {
            await repository.UpdateAsync(entity);
            await uow.CommitAsync();
        }
    }

    public async Task SaveAsync(TimeAddedDetailModel model, Guid playlistId)
    {
        TimeAddedEntity entity = timeAddedModelMapper.MapToEntity(model, playlistId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<TimeAddedEntity> repository =
            uow.GetRepository<TimeAddedEntity, TimeAddedEntityMapper>();

        repository.Insert(entity);
        await uow.CommitAsync();
    }

    public async Task<IEnumerable<TimeAddedListModel>> GetByPlaylistIdAsync(Guid playlistId, TimeAddedFilter? filter = null)
    {
        await using var uow = UnitOfWorkFactory.Create();
        IQueryable<TimeAddedEntity> query = uow.GetRepository<TimeAddedEntity, TimeAddedEntityMapper>().Get()
            .Include(t => t.MultimediaFile)
            .Where(t => t.PlaylistId == playlistId);

        if (filter != null)
        {
            if (filter.AddedAfter.HasValue)
            {
                query = query.Where(t => t.DateTime >= filter.AddedAfter.Value);
            }

            if (filter.AddedBefore.HasValue)
            {
                query = query.Where(t => t.DateTime <= filter.AddedBefore.Value);
            }

            if (filter.FileType.HasValue)
            {
                query = query.Where(t => t.MultimediaFile.FileType == filter.FileType);
            }
        }

        var entities = await query.ToListAsync();
        return ModelMapper.MapToListModel(entities);
    }
}

