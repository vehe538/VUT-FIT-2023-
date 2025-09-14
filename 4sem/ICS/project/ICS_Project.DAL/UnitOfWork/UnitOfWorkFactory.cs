using Microsoft.EntityFrameworkCore;

namespace ICS_Project.DAL.UnitOfWork;

public class UnitOfWorkFactory(IDbContextFactory<MediaPlayerDbContext> dbContextFactory) : IUnitOfWorkFactory
{
    public IUnitOfWork Create() => new UnitOfWork(dbContextFactory.CreateDbContext());
}