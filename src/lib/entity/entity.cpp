

#include <iostream>
#include <map>
#include <vector>
#include "entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

Entity::Entity(size_t id, const std::string &tag) : m_tag(tag), m_id(id) {};

const bool Entity::isAlive() const
{
    return m_alive;
}
void Entity::destroy()
{
    m_alive = false;
}
const std::string &Entity::tag() const
{
    return m_tag;
}

const size_t Entity::id() const
{
    return m_id;
};
