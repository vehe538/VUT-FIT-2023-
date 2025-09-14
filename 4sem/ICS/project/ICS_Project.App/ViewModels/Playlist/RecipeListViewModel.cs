using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;

namespace ICS_Project.App.ViewModels;

public partial class RecipeListViewModel(
    IPlaylistFacade recipeFacade,
    INavigationService navigationService,
    IMessengerService messengerService)
    : ViewModelBase(messengerService), IRecipient<PlaylistEditMessage>, IRecipient<PlaylistDeleteMessage>
{
    [ObservableProperty]
    private IEnumerable<PlaylistListModel> _playlists = [];

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Playlists = await recipeFacade.GetAsync();
    }

    [RelayCommand]
    private async Task GoToDetailAsync(Guid id)
        => await navigationService.GoToAsync(NavigationService.PlaylistDetailRouteRelative,
            new Dictionary<string, object?>
            {
                [nameof(PlaylistDetailViewModel.Id)] = id
            }
        );

    [RelayCommand]
    private async Task GoToCreateAsync()
    {
        await navigationService.GoToAsync(NavigationService.PlaylistEditRouteRelative);
    }

    public void Receive(PlaylistEditMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }

    public void Receive(PlaylistDeleteMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }
}