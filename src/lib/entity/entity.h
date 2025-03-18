#ifndef ENTITY_H
#define ENTITY_H
#include <map>
#include <vector>
#include "components/components.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;
class Entity
{ //  friend class EntityManager;
    const size_t m_id = 1;
    const std::string m_tag = "default";
    bool m_alive = true;

public:
    std::shared_ptr<CTransform> CTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;
    Entity(size_t id, const std::string &tag);
    const bool isAlive() const;
    const size_t id() const;
    const std::string &tag() const;
    void destroy();
};

#endif
