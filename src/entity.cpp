

#include <iostream>
#include <map>
#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class Entity
{
    friend EntityManager;
    const size_t m_id = 1;
    const std::string m_tag = "Default";
    bool m_alive = true;
    Entity(const std::string &tag, size_t id) : m_tag(tag), m_id(id) {};

public:
    // components here
    const bool getAliveStatus() const
    {
        return m_alive;
    }
    void destroy()
    {
        m_alive = false;
    }
    const std::string &getTag()
    {
        return m_tag;
    }
};

int main(int argc, char *argv[])
{

    return 0;
}