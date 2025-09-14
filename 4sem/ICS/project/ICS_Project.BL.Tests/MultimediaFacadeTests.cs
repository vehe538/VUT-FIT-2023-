using FluentAssertions;
using ICS_Project.BL.Facades.Filters;
using ICS_Project.BL.Mappers;
using ICS_Project.BL.Models;
using ICS_Project.BL.Facades;
using ICS_Project.Common.Enums;
using ICS_Project.DAL.Seeds;
using Microsoft.EntityFrameworkCore;
using Xunit.Abstractions;

namespace ICS_Project.BL.Tests;

public class MultimediaFileFacadeTests : FacadeTestsBase
{
    private readonly IMultimediaFileFacade _facade;

    public MultimediaFileFacadeTests(ITestOutputHelper output) : base(output)
    {
        _facade = new MultimediaFileFacade(UnitOfWorkFactory, MultimediaFileModelMapper);
    }

    [Fact]
    public async Task Create_WithNonExistingFile_DoesNotThrow()
    {
        // Arrange
        var model = new MultimediaFileDetailModel
        {
            Id = Guid.NewGuid(),          
            Name = "Example.mp4",
            Author = "sss",
            Url = "https://example.com/file.mp4",
            Size = 123456,
            Duration = 300,
            FileType = FileType.Video,
            Description = "Ukázkové video",
            PictureUrl = "https://example.com/thumb.jpg"
        };

        // Act
        var result = await _facade.SaveAsync(model);

        // Assert
        result.Should().NotBeNull();
    }

    [Fact]
    public async Task GetAll_Single_SeededAudioFile_Kabati()
    {
        // Act
        var files = await _facade.GetAsync();

       
        var file = files.Single(f => f.Id == MultimediaFileSeeds.Kabati.Id);

        // Assert
        var mapper = new MultimediaFileModelMapper();
        file.Should().BeEquivalentTo(mapper.MapToListModel(MultimediaFileSeeds.Kabati));

    }


    [Fact]
    public async Task GetById_SeededVideoFile()
    {
        // Act
        var file = await _facade.GetAsync(MultimediaFileSeeds.KarelGott.Id);

        // Assert
        var mapper = new MultimediaFileModelMapper();
        file.Should().BeEquivalentTo(mapper.MapToDetailModel(MultimediaFileSeeds.KarelGott));
    }
    

    [Fact]
    public async Task GetById_NonExistent()
    {
        // Arrange
        var nonExistentId = Guid.NewGuid();

        // Act
        var file = await _facade.GetAsync(nonExistentId);

        // Assert
        file.Should().BeNull();
    }


    [Fact]
    public async Task SeededAudioFile_DeleteById_Deleted()
    {
        // Act
        await _facade.DeleteAsync(MultimediaFileSeeds.KarelGott.Id);

        // Assert
        await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
        var exists = await dbxAssert.MultimediaFiles.AnyAsync(f => f.Id == MultimediaFileSeeds.KarelGott.Id);

        exists.Should().BeFalse();
    }

    [Fact]
    public async Task GetFilteredAsync_FilterByType_ReturnsVideos()
    {
        // Arrange
        var filter = new MultimediaFileFilter { FileType = FileType.Video };

        // Act
        var result = await _facade.GetFilteredAsync(filter);

        // Assert
        result.Should().HaveCount(1);
        result.Should().Contain(f => f.Id == MultimediaFileSeeds.JanGott.Id);

    }
}