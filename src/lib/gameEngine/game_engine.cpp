// toplevel game object
// holds all game data

class GameEngine
{
    EntityManager m_entityManager;
    size_t m_currentFrame = 0;
    Entity m_player;
    bool m_paused;
    bool m_running;

public:
    GameEngine();
    void mainLoop()
    {
        m_entityManager.update();
        // sUserInput();
        // sMovement();
        // sCollision();
        // sRender();
        m_currentFrame++;
    }
};