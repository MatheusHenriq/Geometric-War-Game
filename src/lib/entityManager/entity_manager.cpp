#include <iostream>
#include <map>
#include <vector>
#include "entity_manager.h"
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

EntityManager::EntityManager() {}

void EntityManager::update()
{
    for (auto e : m_toAdd)
    {
        m_entities.push_back(e);
    }

    removeDeadEntities(m_entities);
    m_entityMap.clear();

    for (auto e : m_entities)
    {
        m_entityMap[e->tag()].push_back(e); // if "tag" does not exists c++ will create it for us.
    }
    m_toAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
    auto deadEntities = std::remove_if(vec.begin(), vec.end(), [](Entity v)
                                       { return !v.isAlive(); });

    vec.erase(deadEntities);
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto e = std::make_shared<Entity>(new Entity(m_totalEntities++, tag));
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
