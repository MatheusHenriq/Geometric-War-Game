// toplevel game object
// holds all game data
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "../entityManager/entity_manager.h"
#include "../entity/entity.h"
#include "../vec2/vec2.h"
#include "game_engine.h"
#include <sstream>

GameEngine::GameEngine(const std::string &config)
{
    init(config);
}

void GameEngine::loadConfigFile(const std::string &config)
{
    std::string type;
    std::ifstream fin(config);
    while (fin >> type)
    {
        if (type == "Window")
        {
            fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
        }
        else if (type == "Font")
        {
            fin >> m_fontConfig.P >> m_fontConfig.ST >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
        }
        else if (type == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
        }
        else if (type == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        else if (type == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        }
        else
        {
            fin.close();
        }
    }
}

void GameEngine::init(const std::string &config)
{
    loadConfigFile(config);
    if (m_font.openFromFile(m_fontConfig.P))
    {
        m_text = std::make_shared<sf::Text>(m_font);
        m_text->setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
        m_text->setCharacterSize(m_fontConfig.ST);
        m_text->setPosition(sf::Vector2f(12.0f, 0.0f));
        m_text->setString("Score: " + std::to_string(m_score));
    }
    if (m_windowConfig.FS == 0)
    {
        m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2!");
    }
    else
    {
        m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2!", sf::State::Fullscreen);
    }
    spectialAbilityCoolDown.start();
    m_window.setFramerateLimit(m_windowConfig.FL);

    spawnPlayer();
}

void GameEngine::sLifespan()
{

    for (auto e : m_entities.getEntities("bullet"))
    {
        e->cLifespan->remaining--;
        sf::Color outlineColor = e->cShape->circle.getOutlineColor();
        sf::Color fillColor = e->cShape->circle.getFillColor();
        e->cShape->circle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, 255 * e->cLifespan->remaining / e->cLifespan->total));
        e->cShape->circle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, 255 * e->cLifespan->remaining / e->cLifespan->total));
        if (e->cLifespan->remaining == 0)
        {
            e->destroy();
        }
    }
    for (auto e : m_entities.getEntities("smallEnemy"))
    {
        e->cLifespan->remaining--;
        sf::Color outlineColor = e->cShape->circle.getOutlineColor();
        sf::Color fillColor = e->cShape->circle.getFillColor();
        e->cShape->circle.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, 255 * e->cLifespan->remaining / e->cLifespan->total));
        e->cShape->circle.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, 255 * e->cLifespan->remaining / e->cLifespan->total));
        if (e->cLifespan->remaining == 0)
        {
            e->destroy();
        }
    }
}

void GameEngine::sCollision()
{
    // Check for collision bullet - enemy

    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            float distanceSquare = e->CTransform->pos.distSquare(b->CTransform->pos);
            float radiusSquareSum = (b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius);
            if (abs(distanceSquare) <= radiusSquareSum)
            {
                e->destroy();
                b->destroy();

                m_score += e->cScore->score;
                spawnSmallEnemies(e);
                break;
            }
        }
        for (auto e : m_entities.getEntities("smallEnemy"))
        {
            float distanceSquare = e->CTransform->pos.distSquare(b->CTransform->pos);
            float radiusSquareSum = (b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius);
            if (abs(distanceSquare) <= radiusSquareSum)
            {
                e->destroy();
                b->destroy();
                m_score += e->cScore->score;
                break;
            }
        }
    }
    // Check for collision on borders and Collision with player
    float wx = m_window.getSize().x;
    float wy = m_window.getSize().y;
    for (auto e : m_entities.getEntities("enemy"))
    {
        float radius = e->cShape->circle.getRadius();

        if (e->CTransform->pos.y - radius <= 0 || e->CTransform->pos.y + radius >= wy)
        {
            e->CTransform->velocity.y *= -1;
        }
        if (e->CTransform->pos.x - radius <= 0 || e->CTransform->pos.x + radius >= wx)
        {
            e->CTransform->velocity.x *= -1;
        }

        float distanceSquare = e->CTransform->pos.distSquare(m_player->CTransform->pos);
        float radiusSquareSum = (m_player->cCollision->radius + e->cCollision->radius) * (m_player->cCollision->radius + e->cCollision->radius);
        if (abs(distanceSquare) <= radiusSquareSum)
        {
            e->destroy();
            m_player->CTransform->pos = Vec2(wx / 2.0f, wy / 2.0f);
            m_score = 0;
            spawnSmallEnemies(e);
            break;
        }
    }
}

void GameEngine::run()
{
    while (m_running)
    {
        m_entities.update();
        sRender();

        if (!m_paused)
        {
            sEnemySpawner();
            sLifespan();
            sCollision();
            sMoviment();
            m_currentFrame++;
        }

        sUserInput();
    }
}

int GameEngine::randNumber(const int max, const int min)
{
    return min + (std::rand() % (1 + max - min));
}

void GameEngine::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos)
{
    auto e = m_entities.addEntity("bullet");
    Vec2 diffVec = mousePos - entity->CTransform->pos;
    float diffVecLenght = diffVec.length();
    diffVec /= diffVecLenght;
    Vec2 entityPosition = entity->CTransform->pos;
    e->CTransform = std::make_shared<CTransform>(Vec2(entityPosition.x, entityPosition.y),
                                                 Vec2(diffVec.x * m_bulletConfig.S, diffVec.y * m_bulletConfig.S),
                                                 0.0f);

    e->cShape = std::make_shared<CShape>(m_bulletConfig.SR,
                                         m_bulletConfig.V,
                                         sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                                         sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OG),
                                         m_bulletConfig.OT);
    e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void GameEngine::sEnemySpawner()
{
    spawnEnemy();
}

void GameEngine::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
    int smallEnemiesNumber = entity->cShape->circle.getPointCount();
    float angle = 2 * M_PI / smallEnemiesNumber;
    for (size_t i = 0; i < smallEnemiesNumber; i++)
    {

        auto e = m_entities.addEntity("smallEnemy");
        float velocity = entity->CTransform->velocity.x > entity->CTransform->velocity.y ? entity->CTransform->velocity.x : entity->CTransform->velocity.y;
        e->CTransform = std::make_shared<CTransform>(Vec2(entity->CTransform->pos.x, entity->CTransform->pos.y),
                                                     Vec2(((velocity * cos(angle * i))) / m_enemyConfig.SMAX, (velocity * sin(angle * i)) / m_enemyConfig.SMAX),
                                                     0.0f);
        e->cShape = std::make_shared<CShape>(m_enemyConfig.SR / 2.5,
                                             smallEnemiesNumber,
                                             entity->cShape->circle.getFillColor(),
                                             entity->cShape->circle.getOutlineColor(),
                                             m_enemyConfig.OT);
        e->cCollision = std::make_shared<CCollision>(m_enemyConfig.SR / 2.5);
        e->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
        e->cScore = std::make_shared<CScore>(100 * smallEnemiesNumber);
    }
}

void GameEngine::spawnEnemy()
{

    if (m_currentFrame - m_lastEnemySpawnTime == m_enemyConfig.SI)
    {
        auto enemyVerticesNumber = randNumber(m_enemyConfig.VMAX, m_enemyConfig.VMIN);
        m_lastEnemySpawnTime = m_currentFrame;
        float ex = randNumber(m_window.getSize().x - m_enemyConfig.SR, m_enemyConfig.SR);
        float ey = randNumber(m_window.getSize().y - m_enemyConfig.SR, m_enemyConfig.SR);
        if (abs(Vec2(ex, ey).dist(m_player->CTransform->pos)) <= m_playerConfig.CR)
        {
            float playerCR = m_playerConfig.CR * m_playerConfig.S * 2;
            if (ex + playerCR < m_window.getSize().x)
            {
                ex += playerCR;
            }
            else if (ex + playerCR > m_window.getSize().x)
            {
                ex -= playerCR;
            }
            if (ey + playerCR < m_window.getSize().y)
            {
                ey += playerCR;
            }
            else if (ey + playerCR > m_window.getSize().y)
            {
                ey -= playerCR;
            }
        }
        float evx = float(randNumber(m_enemyConfig.SMAX, m_enemyConfig.SMIN));
        float evy = float(randNumber(m_enemyConfig.SMAX, m_enemyConfig.SMIN));
        int er = randNumber(255, 0);
        int rg = randNumber(255, 0);
        int eb = randNumber(255, 0);

        auto entity = m_entities.addEntity("enemy");
        entity->CTransform = std::make_shared<CTransform>(Vec2(ex, ey),
                                                          Vec2(evx, evy),
                                                          0.0f);
        entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR,
                                                  enemyVerticesNumber,
                                                  sf::Color(er, rg, eb),
                                                  sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OG),
                                                  m_enemyConfig.OT);
        entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
        entity->cLifespan = std::make_shared<CLifespan>(0);
        entity->cScore = std::make_shared<CScore>(100 * enemyVerticesNumber);
    }
}

void GameEngine::spawnPlayer()
{
    auto entity = m_entities.addEntity("player");
    float mx = m_window.getSize().x / 2.0f;
    float my = m_window.getSize().y / 2.0f;
    entity->CTransform = std::make_shared<CTransform>(Vec2(mx, my),
                                                      Vec2(0.0f, 0.0f),
                                                      0.0f);
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SR,
                                              m_playerConfig.V,
                                              sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
                                              sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OG),
                                              m_playerConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
}

void GameEngine::sRender()
{
    m_window.clear();
    m_text->setString("Score:" + std::to_string(m_score));

    for (auto e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(sf::Vector2(e->CTransform->pos.x, e->CTransform->pos.y));
        e->CTransform->angle += 1.0f;
        e->cShape->circle.setRotation(sf::degrees(e->CTransform->angle));
        m_window.draw(e->cShape->circle);

        m_window.draw(*m_text);
    }

    m_window.display();
}

void GameEngine::sUserInput()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {

                Vec2 mouseVec = Vec2(float(event->getIf<sf::Event::MouseButtonPressed>()->position.x), float(event->getIf<sf::Event::MouseButtonPressed>()->position.y));
                spawnBullet(m_player, mouseVec);
            }
            else if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right)
            {
                spawnSpecialWeapon(m_player);
            }
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            switch (event->getIf<sf::Event::KeyPressed>()->code)
            {
            case sf::Keyboard::Key::Escape:
                m_running = false;
                m_window.close();
            case sf::Keyboard::Key::W:
                m_player->cInput->up = true;
                break;

            case sf::Keyboard::Key::S:
                m_player->cInput->down = true;
                break;

            case sf::Keyboard::Key::A:
                m_player->cInput->left = true;
                break;

            case sf::Keyboard::Key::D:
                m_player->cInput->right = true;
                break;

            case sf::Keyboard::Key::P:
                setPause(!m_paused);
                break;

            default:
                break;
            }
        }
        if (event->is<sf::Event::KeyReleased>())
        {
            switch (event->getIf<sf::Event::KeyReleased>()->code)
            {
            case sf::Keyboard::Key::W:
                m_player->cInput->up = false;
                break;

            case sf::Keyboard::Key::S:
                m_player->cInput->down = false;
                break;

            case sf::Keyboard::Key::A:
                m_player->cInput->left = false;
                break;

            case sf::Keyboard::Key::D:
                m_player->cInput->right = false;
                break;

            default:
                break;
            }
        }
    }
}

void GameEngine::sMoviment()
{
    float wx = m_window.getSize().x;
    float wy = m_window.getSize().y;

    m_player->CTransform->velocity = Vec2(0, 0);

    for (auto e : m_entities.getEntities())
    {

        if (e->tag() != "player")
        {
            e->CTransform->pos.x += e->CTransform->velocity.x;
            e->CTransform->pos.y += e->CTransform->velocity.y;
        }
        else
        {
            if (m_player->cInput->up && m_player->CTransform->pos.y - m_player->cShape->circle.getRadius() > 0)
            {
                m_player->CTransform->velocity.y = -m_playerConfig.S;
            }
            if (m_player->cInput->down && m_player->CTransform->pos.y + m_player->cShape->circle.getRadius() < wy)
            {
                m_player->CTransform->velocity.y = m_playerConfig.S;
            }
            if (m_player->cInput->left && m_player->CTransform->pos.x - m_player->cShape->circle.getRadius() > 0)
            {
                m_player->CTransform->velocity.x = -m_playerConfig.S;
            }
            if (m_player->cInput->right && m_player->CTransform->pos.x + m_player->cShape->circle.getRadius() < wx)
            {
                m_player->CTransform->velocity.x = m_playerConfig.S;
            }

            m_player->CTransform->pos.x += m_player->CTransform->velocity.x;
            m_player->CTransform->pos.y += m_player->CTransform->velocity.y;
        }
    }
}

void GameEngine::setPause(bool paused)
{
    m_paused = paused;
}

void GameEngine::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    if (!m_paused)
    {
        sf::Time elapsedTime = spectialAbilityCoolDown.getElapsedTime();
        if (elapsedTime.asMilliseconds() >= 2000)
        {
            Vec2 entityPosition = entity->CTransform->pos;

            float angle = 2 * M_PI / (m_playerConfig.V + 5);

            for (size_t i = 0; i < (m_playerConfig.V + 5); i++)
            {
                auto e = m_entities.addEntity("bullet");
                e->CTransform = std::make_shared<CTransform>(Vec2(entityPosition.x, entityPosition.y),
                                                             Vec2(m_playerConfig.S * cos(angle * i), m_playerConfig.S * sin(angle * i)),
                                                             0.0f);

                e->cShape = std::make_shared<CShape>(m_bulletConfig.SR,
                                                     m_bulletConfig.V,
                                                     m_player->cShape->circle.getFillColor(),
                                                     m_player->cShape->circle.getOutlineColor(),
                                                     m_bulletConfig.OT);

                e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
                e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
                spectialAbilityCoolDown.restart();
            }
        }
    }
}
