using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.App.ViewModels;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;

namespace ICS_Project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class MultimediaFileEditViewModel(
    IMultimediaFileFacade multimediaFileFacade,
    INavigationService navigationService,
    IMessengerService messengerService)
    : ViewModelBase(messengerService)
{
    public Guid Id { get; set; }

    [ObservableProperty]
    private MultimediaFileDetailModel _multimediaFile = MultimediaFileDetailModel.Empty;

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        MultimediaFile = await multimediaFileFacade.GetAsync(Id)
                     ?? MultimediaFileDetailModel.Empty;
    }

    [RelayCommand]
    private async Task SaveAsync()
    {
        await multimediaFileFacade.SaveAsync(MultimediaFile);

        MessengerService.Send(new IngredientEditMessage { MultimediaFileId = MultimediaFile.Id });

        navigationService.SendBackButtonPressed();
    }
}