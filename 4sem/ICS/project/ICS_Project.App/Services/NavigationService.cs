using ICS_Project.App.Models;
using ICS_Project.App.Views.MultimediaFile;
using ICS_Project.App.Views.Playlist;

namespace ICS_Project.App.Services;

public class NavigationService : INavigationService
{
    public const string PlaylistListRouteAbsolute = "//playlists";
    public const string PlaylistDetailRouteRelative = "/detail";
    public const string PlaylistEditRouteRelative = "/edit";
    public const string PlaylistFilesEditRouteRelative = "/multimediafiles";

    public const string MultimediaFileListRouteAbsolute = "//multimediafiles";
    public const string MultimediaFileDetailRouteRelative = "/detail";
    public const string MultimediaFileEditRouteRelative = "/edit";

    public IEnumerable<RouteModel> Routes { get; } = new List<RouteModel>
    {
        new(MultimediaFileListRouteAbsolute, typeof(MultimediaFileListView)),
        new(MultimediaFileListRouteAbsolute + MultimediaFileDetailRouteRelative, typeof(MultimediaFileDetailView)),

        new(MultimediaFileListRouteAbsolute + MultimediaFileEditRouteRelative, typeof(MultimediaFileEditView)),
        new(MultimediaFileListRouteAbsolute + MultimediaFileDetailRouteRelative + MultimediaFileEditRouteRelative, typeof(MultimediaFileEditView)),

        new(PlaylistListRouteAbsolute, typeof(PlaylistListView)),
        new(PlaylistListRouteAbsolute + PlaylistDetailRouteRelative, typeof(PlaylistDetailView)),

        new(PlaylistListRouteAbsolute + PlaylistEditRouteRelative, typeof(PlaylistEditView)),
        new(PlaylistListRouteAbsolute + PlaylistDetailRouteRelative + PlaylistEditRouteRelative, typeof(PlaylistEditView)),

        new(PlaylistListRouteAbsolute + PlaylistDetailRouteRelative + PlaylistEditRouteRelative + PlaylistFilesEditRouteRelative, typeof(PlaylistFilesEditView)),
    };

    public async Task GoToAsync(string route)
        => await Shell.Current.GoToAsync(route);

    public async Task GoToAsync(string route, IDictionary<string, object?> parameters)
        => await Shell.Current.GoToAsync(route, parameters);

    public bool SendBackButtonPressed()
        => Shell.Current.SendBackButtonPressed();
}