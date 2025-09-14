using ICS_Project.Common.Enums;

namespace ICS_Project.BL.Facades.Filters;
public class MultimediaFileFilter
{
    public string? Search { get; set; }
    public FileType? FileType { get; set; }
    public int? MinDuration { get; set; }
    public int? MaxDuration { get; set; }
    public string? SortBy { get; set; } = "Title";
    public bool Descending { get; set; } = false;
}

