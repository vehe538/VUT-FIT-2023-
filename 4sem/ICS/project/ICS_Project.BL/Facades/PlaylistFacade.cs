using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;
using ICS_Project.DAL.Mappers;
using ICS_Project.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICS_Project.BL.Facades;
public class PlaylistFacade(
    IUnitOfWorkFactory unitOfWorkFactory,
    PlaylistModelMapper modelMapper)
    : FacadeBase<PlaylistEntity, PlaylistListModel, PlaylistDetailModel, PlaylistEntityMapper>(unitOfWorkFactory, modelMapper),
        IPlaylistFacade
{
    protected override ICollection<string> IncludesNavigationPathDetail =>
        new[] { $"{nameof(PlaylistEntity.MultimediaFiles)}.{nameof(TimeAddedEntity.MultimediaId)}" };

    public async Task<IEnumerable<PlaylistListModel>> GetFilteredAsync(PlaylistFilter filter)
    {
        await using var uow = UnitOfWorkFactory.Create();
        IQueryable<PlaylistEntity> query = uow.GetRepository<PlaylistEntity, PlaylistEntityMapper>().Get();

        if (!string.IsNullOrEmpty(filter.Search))
        {
            query = query.Where(p => p.Name.Contains(filter.Search));
        }

        query = filter.SortBy?.ToLower() switch
        {
            "name" => filter.Descending
                ? query.OrderByDescending(p => p.Name)
                : query.OrderBy(p => p.Name),

            "id" => filter.Descending
                ? query.OrderByDescending(p => p.Id)
                : query.OrderBy(p => p.Id),

            _ => query.OrderBy(p => p.Name)
        };

        var entities = await query.ToListAsync();
        return ModelMapper.MapToListModel(entities);
    }
}
