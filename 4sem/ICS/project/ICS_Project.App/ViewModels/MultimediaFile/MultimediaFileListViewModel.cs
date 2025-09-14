using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ICS_Project.App.Messages;
using ICS_Project.App.Services;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Models;

namespace ICS_Project.App.ViewModels;

public partial class MultimediaFileListViewModel(
    IMultimediaFileFacade multimediaFileFacade,
    INavigationService navigationService,
    IMessengerService messengerService)
    : ViewModelBase(messengerService), IRecipient<IngredientEditMessage>, IRecipient<MultimediaFileDeleteMessage>
{
    [ObservableProperty]
    private IEnumerable<MultimediaFileListModel> _multimediaFile = [];

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        MultimediaFile = await multimediaFileFacade.GetAsync();
    }

    [RelayCommand]
    private async Task GoToCreateAsync()
    {
        await navigationService.GoToAsync(NavigationService.MultimediaFileEditRouteRelative);
    }

    [RelayCommand]
    private async Task GoToDetailAsync(Guid id)
    {
        await navigationService.GoToAsync(NavigationService.MultimediaFileDetailRouteRelative,
            new Dictionary<string, object?>
            {
                [nameof(MultimediaFileDetailViewModel.Id)] = id
            }
        );
    }

    public void Receive(IngredientEditMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }

    public void Receive(MultimediaFileDeleteMessage message)
    {
        ForceDataRefreshOnNextAppearing();
    }
}

