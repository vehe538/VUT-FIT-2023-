using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using ICS_Project.Common.Enums;
//using Kotlin;
using System.Collections.ObjectModel;

namespace ICS_Project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class PlaylistFilesEditViewModel(
    IMultimediaFileFacade multimediaFileFacade,
    ITimeAddedFacade timeAddedFacade,
    IPlaylistFacade playlistFacade,
    TimeAddedModelMapper timeAddedModelMapper,
    IMessengerService messengerService)
    : ViewModelBase(messengerService)
{
    public Guid Id { get; set; }

    //public List<Unit> Units { get; set; } = [.. (Unit[])Enum.GetValues(typeof(Unit))];
        // idk what its for. Download nuget package and uncomment if needed

    [ObservableProperty]
    private PlaylistDetailModel? _playlist;

    [ObservableProperty]
    private ObservableCollection<MultimediaFileListModel> _multimediaFiles = new();

    [ObservableProperty]
    private MultimediaFileListModel? _fileSelected;

    [ObservableProperty]
    private TimeAddedDetailModel? _timeAddedNew;

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Playlist = await playlistFacade.GetAsync(Id)
            ?? PlaylistDetailModel.Empty;

        MultimediaFiles.Clear();
        var multimediaFiles = await multimediaFileFacade.GetAsync();
        foreach (var file in multimediaFiles)
        {
            MultimediaFiles.Add(file);
            TimeAddedNew = GetIngredientAmountNew();
        }
    }

    [RelayCommand]
    private async Task AddNewIngredientToRecipeAsync()
    {
        if (TimeAddedNew is not null
            && FileSelected is not null
            && Playlist is not null)
        {
            timeAddedModelMapper.MapToExistingDetailModel(TimeAddedNew, FileSelected);

            await timeAddedFacade.SaveAsync(TimeAddedNew, Playlist.Id);
            Playlist.MultimediaFiles.Add(timeAddedModelMapper.MapToListModel(TimeAddedNew));

            TimeAddedNew = GetIngredientAmountNew();

            MessengerService.Send(new PlaylistFileAddMessage());
        }
    }

    [RelayCommand]
    private async Task UpdateIngredientAsync(TimeAddedListModel? model)
    {
        if (model is not null
            && Playlist is not null)
        {
            await timeAddedFacade.SaveAsync(model, Playlist.Id);

            MessengerService.Send(new PlaylistFileEditMessage());
        }
    }

    [RelayCommand]
    private async Task RemoveIngredientAsync(TimeAddedListModel model)
    {
        if (Playlist is not null)
        {
            await timeAddedFacade.DeleteAsync(model.Id);
            Playlist.MultimediaFiles.Remove(model);

            MessengerService.Send(new PlaylistFileDeleteMessage());
        }
    }

    private TimeAddedDetailModel GetIngredientAmountNew()
    {
        var multimediaFileFirst = MultimediaFiles.First();
        return new()
        {
            Id = Guid.NewGuid(),
            MultimediaId = multimediaFileFirst.Id,
            MultimediaName = multimediaFileFirst.Name,
            MultimediaSize = multimediaFileFirst.Size,
            MultimediaDuration = multimediaFileFirst.Duration,
            MultimediaAuthor = multimediaFileFirst.Author,
            MultimediaUrl = multimediaFileFirst.Url,
            MultimediaFileType = multimediaFileFirst.FileType,
            MultimediaDescription = multimediaFileFirst.Description,
            MultimediaPictureUrl = multimediaFileFirst.PictureUrl

        };
    }
}