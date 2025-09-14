using FluentAssertions;
using ICS_Project.BL.Facades;
using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using Xunit.Abstractions;

namespace ICS_Project.BL.Tests;

public class PlaylistFacadeTests : FacadeTestsBase
{
    private readonly IPlaylistFacade _facade;

    public PlaylistFacadeTests(ITestOutputHelper output) : base(output)
    {
        _facade = new PlaylistFacade(UnitOfWorkFactory, PlaylistModelMapper);
    }

    [Fact]
    public async Task GetFilteredAsync_WithNameSearch_ReturnsMatchingPlaylist()
    {
        // Arrange
        var playlist = new PlaylistDetailModel
        {
            Id = Guid.NewGuid(),
            Name = "Name",
            Description = "Description",
        };

        var playlist1 = new PlaylistDetailModel
        {
            Id = Guid.NewGuid(),
            Name = "Chill",
            Description = "Description",
        };

        await _facade.SaveAsync(playlist);
        await _facade.SaveAsync(playlist1);

        var filter = new PlaylistFilter
        {
            Search = "Chill",
            SortBy = "name",
            Descending = false
        };

        // Act
        var result = (await _facade.GetFilteredAsync(filter)).ToList();

        // Assert
        result.Should().ContainSingle();
        result.First().Name.Should().Be("Chill");
    }

    [Fact]
    public async Task GetFilteredAsync_WithEmptySearch_ReturnsAllPlaylistsSortedByName()
    {
        // Arrange
        await _facade.SaveAsync(new PlaylistDetailModel { Id = Guid.NewGuid(), Name = "Jazz", Description = "Desciription" });
        await _facade.SaveAsync(new PlaylistDetailModel { Id = Guid.NewGuid(), Name = "Ambient", Description = "Description" });
        await _facade.SaveAsync(new PlaylistDetailModel { Id = Guid.NewGuid(), Name = "Rock", Description= "Description" });

        var filter = new PlaylistFilter
        {
            Search = "",
            SortBy = "name",
            Descending = false
        };

        // Act
        var result = (await _facade.GetFilteredAsync(filter)).ToList();

        // Assert
        result.Should().HaveCountGreaterThanOrEqualTo(3);
        result.Should().BeInAscendingOrder(p => p.Name);
    }

    [Fact]
    public async Task GetFilteredAsync_SortByIdDescending_ReturnsSortedCorrectly()
    {
        // Arrange
        var p1 = await _facade.SaveAsync(new PlaylistDetailModel { Id = Guid.NewGuid(), Name = "Alpha", Description = "Desciription" });
        var p2 = await _facade.SaveAsync(new PlaylistDetailModel { Id = Guid.NewGuid(), Name = "Beta", Description = "Desciription" });

        var filter = new PlaylistFilter
        {
            SortBy = "Name",
            Descending = true
        };

        // Act
        var result = (await _facade.GetFilteredAsync(filter)).ToList();

        // Assert
        result.First().Id.Should().Be(p2.Id);
    }
}