﻿using ICS_Project.DAL.Entities;

namespace ICS_Project.DAL.Repositories;

public interface IRepository<TEntity>
    where TEntity : class, IEntity
{
    IQueryable<TEntity> Get();
    Task DeleteAsync(Guid entityId);
    ValueTask<bool> ExistsAsync(TEntity entity);
    TEntity Insert(TEntity entity);
    Task<TEntity> UpdateAsync(TEntity entity);
}