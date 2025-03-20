#include <iostream>
#include <map>
#include <vector>
#include "entity_manager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{
    for (auto e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    size_t oldLength = m_entities.size();
    removeDeadEntities(m_entities);
    size_t newLength = m_entities.size();
    if (newLength != oldLength)
    {
        m_entityMap.clear();
        for (auto e : m_entities)
        {
            m_entityMap[e->tag()].push_back(e);
        }
    }

    m_toAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
    auto deadEntities = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity> &v)
                                       { return !v->isAlive(); });

    vec.erase(deadEntities, vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto e = std::make_shared<Entity>(m_totalEntities++, tag);
    m_toAdd.push_back(e);
    return e;
}

EntityVec &EntityManager::getEntities()
{
    return m_entities;
}

EntityVec &EntityManager::getEntities(const std::string &tag)
{
    return m_entityMap[tag];
}
