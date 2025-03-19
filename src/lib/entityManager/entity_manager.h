#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <iostream>
#include <map>
#include <vector>
class Entity; // forward dclaration
#include "../entity/entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

public:
    EntityManager();
    // called at beggining of each frame by game engine
    // entities added will now be available to use this frame
    void update();
    void removeDeadEntities(EntityVec &vec);
    std::shared_ptr<Entity> addEntity(const std::string &tag);
    EntityVec &getEntities();
    EntityVec &getEntities(const std::string &tag);
};

#endif