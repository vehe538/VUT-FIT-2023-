using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;
using ICS_Project.Common.Enums;

namespace ICS_Project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class PlaylistEditViewModel(
    IPlaylistFacade recipeFacade,
    INavigationService navigationService,
    IMessengerService messengerService)
    : ViewModelBase(messengerService), IRecipient<PlaylistFileEditMessage>, IRecipient<PlaylistFileAddMessage>,
        IRecipient<PlaylistFileDeleteMessage>
{
    public Guid Id { get; set; }

    [ObservableProperty]
    private PlaylistDetailModel _playlist = PlaylistDetailModel.Empty;

    public List<FileType> FoodTypes { get; set; } = [.. (FileType[])Enum.GetValues(typeof(FileType))];

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Playlist = await recipeFacade.GetAsync(Id)
                 ?? PlaylistDetailModel.Empty;
    }

    [RelayCommand]
    private async Task GoToRecipeIngredientEditAsync()
    {
        await navigationService.GoToAsync(NavigationService.PlaylistFilesEditRouteRelative,
            new Dictionary<string, object?>
            {
                [nameof(PlaylistFilesEditViewModel.Id)] = Playlist.Id
            });
    }

    [RelayCommand]
    private async Task SaveAsync()
    {
        await recipeFacade.SaveAsync(Playlist with { MultimediaFiles = default! });

        MessengerService.Send(new PlaylistEditMessage { PlaylistId = Playlist.Id });

        navigationService.SendBackButtonPressed();
    }

    public void Receive(PlaylistFileEditMessage message)
    {
        ForceDataRefreshOnNextAppearing();
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