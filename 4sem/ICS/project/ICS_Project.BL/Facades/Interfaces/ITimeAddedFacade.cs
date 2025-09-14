using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;

namespace ICS_Project.BL.Facades;
public interface ITimeAddedFacade : IFacade<TimeAddedEntity, TimeAddedListModel, TimeAddedDetailModel>
{
    Task SaveAsync(TimeAddedDetailModel model, Guid playlistId);
    Task SaveAsync(TimeAddedListModel model, Guid playlistId);
    Task<IEnumerable<TimeAddedListModel>> GetByPlaylistIdAsync(Guid playlistId, TimeAddedFilter? filter = null);
}
