using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;

namespace ICS_Project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class PlaylistDetailViewModel(
    IPlaylistFacade playlistFacade,
    INavigationService navigationService,
    IMessengerService messengerService)
    : ViewModelBase(messengerService), IRecipient<PlaylistEditMessage>, IRecipient<PlaylistFileAddMessage>,
        IRecipient<PlaylistFileDeleteMessage>
{
    public Guid Id { get; set; }

    [ObservableProperty]
    private PlaylistDetailModel? _playlist;

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Playlist = await playlistFacade.GetAsync(Id);
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (Playlist is not null)
        {
            await playlistFacade.DeleteAsync(Playlist.Id);

            MessengerService.Send(new PlaylistDeleteMessage());

            navigationService.SendBackButtonPressed();
        }
    }


    [RelayCommand]
    private async Task GoToEditAsync()
    {
        if (Playlist is not null)
        {
            await navigationService.GoToAsync(NavigationService.PlaylistEditRouteRelative,
                new Dictionary<string, object?>
                {
                    [nameof(PlaylistEditViewModel.Id)] = Playlist.Id
                }
            );
        }
    }

    public void Receive(PlaylistEditMessage message)
    {
        if (message.PlaylistId == Playlist?.Id)
        {
            ForceDataRefreshOnNextAppearing();
        }
    }

    public void Receive(PlaylistFileAddMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }

    public void Receive(PlaylistFileDeleteMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }
}