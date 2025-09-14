using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Models;
using ICS_Project.DAL.Entities;

namespace ICS_Project.BL.Facades;
public interface IMultimediaFileFacade : IFacade<MultimediaFileEntity, MultimediaFileListModel, MultimediaFileDetailModel>
{
    Task<IEnumerable<MultimediaFileListModel>> GetFilteredAsync(MultimediaFileFilter filter);
}
