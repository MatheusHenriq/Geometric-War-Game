#include <iostream>
#include <map>
#include <vector>

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
    void update()
    {
        for (auto e : m_toAdd)
        {
            m_entities.push_back(e);
        }

        auto deadEntities = std::remove_if(m_entities.begin(), m_entities.end(), [](Entity v)
                                           { return !v.getAliveStatus(); });

        m_entityMap.clear();
        m_entities.erase(deadEntities);

        for (auto e : m_entities)
        {
            m_entityMap[e->tag].push_back(e); // if "tag" does not exists c++ will create it for us.
        }
        m_toAdd.clear();
    };
    std::shared_ptr<Entity> addEntity(const std::string &tag)
    {
        auto e = std::make_shared<Entity>(new Entity(tag, m_totalEntities++));
        m_toAdd.push_back(e);
        return e;
    }
    EntityVec &getEntities();
    EntityVec &getEntities(const std::string &tag);
};
