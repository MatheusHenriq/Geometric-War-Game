// toplevel game object
// holds all game data
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "../entityManager/entity_manager.h"
#include "../entity/entity.h"
#include "../vec2/vec2.h"
#include <SFML/Graphics.hpp>

struct PlayerConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};
struct EnemyConfig
{
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};

struct BulletConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

struct WindowConfig
{
    unsigned int H, W, FL, FS;
};
struct FontConfig
{
    std::string P;
    unsigned int ST, R, G, B;
};
class GameEngine
{
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;

    std::shared_ptr<sf::Text> m_score_text;
    std::shared_ptr<sf::Text> m_pause_text;

    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    sf::Clock spectialAbilityCoolDown;
    BulletConfig m_bulletConfig;
    WindowConfig m_windowConfig;
    FontConfig m_fontConfig;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    std::shared_ptr<Entity> m_player;
    bool m_paused = false;
    bool m_running = true;
    void init(const std::string &config);
    void setPause(bool paused);
    void loadConfigFile(const std::string &config);
    void sRender();
    void sMoviment();
    void sUserInput();
    void sLifespan();
    void sEnemySpawner();
    void sCollision();
    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    int randNumber(const int max, const int min);

public:
    GameEngine(const std::string &config);
    void run();
};

#endif