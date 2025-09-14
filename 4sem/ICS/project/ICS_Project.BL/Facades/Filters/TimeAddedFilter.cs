using ICS_Project.Common.Enums;

namespace ICS_Project.BL.Facades.Filters;
public class TimeAddedFilter
{
    public DateTime? AddedAfter { get; set; }
    public DateTime? AddedBefore { get; set; }
    public FileType? FileType { get; set; }
}

