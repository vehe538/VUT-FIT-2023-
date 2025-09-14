using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;

namespace ICS_Project.BL.Facades;

public interface IPlaylistFacade : IFacade<PlaylistEntity, PlaylistListModel, PlaylistDetailModel>
{
    Task<IEnumerable<PlaylistListModel>> GetFilteredAsync(PlaylistFilter filter);
}
