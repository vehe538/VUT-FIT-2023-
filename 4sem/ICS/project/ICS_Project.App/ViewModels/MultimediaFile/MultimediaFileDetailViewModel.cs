using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;

namespace ICS_Project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class MultimediaFileDetailViewModel(
    IMultimediaFileFacade multimediaFileFacade,
    INavigationService navigationService,
    IMessengerService messengerService,
    IAlertService alertService)
    : ViewModelBase(messengerService), IRecipient<MultimediaFileEditMessage>
{
    public Guid Id { get; set; }

    [ObservableProperty] private MultimediaFileDetailModel? _multimediaFile;

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        MultimediaFile = await multimediaFileFacade.GetAsync(Id);
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (MultimediaFile is not null)
        {
            try
            {
                await multimediaFileFacade.DeleteAsync(MultimediaFile.Id);
                MessengerService.Send(new MultimedeaFileDeleteMessage());
                navigationService.SendBackButtonPressed();
            }
            catch (InvalidOperationException e)
            {
                await alertService.DisplayAsync("Invalid operation",
                    e.Message);
            }
        }
    }

    [RelayCommand]
    private async Task GoToEditAsync()
    {
        if (MultimediaFile?.Id is not null)
        {
            await navigationService.GoToAsync(NavigationService.MultimediaFileEditRouteRelative,
                new Dictionary<string, object?> { [nameof(MultimediaFileEditViewModel.Id)] = MultimediaFile.Id });
        }
    }
    

    public void Receive(MultimediaFileEditMessage message)
    {
        if (message.MultimediaFileId == MultimediaFile?.Id)
        {
            ForceDataRefreshOnNextAppearing();
        }
    }
}