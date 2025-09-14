namespace ICS_Project.BL.Facades.Filters;
public class PlaylistFilter
{
    public string? Search { get; set; }
    public string? SortBy { get; set; } = "Name";
    public bool Descending { get; set; } = false;
}
