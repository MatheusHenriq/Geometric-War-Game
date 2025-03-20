// toplevel game object
// holds all game data
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "../entityManager/entity_manager.h"
#include "../entity/entity.h"
#include "../vec2/vec2.h"
#include "game_engine.h"

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
    }
    if (m_windowConfig.FS == 0)
    {
        m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2!");
    }
    else
    {
        m_window.create(sf::VideoMode({m_windowConfig.W, m_windowConfig.H}), "Assignment 2!", sf::State::Fullscreen);
    }
    m_window.setFramerateLimit(m_windowConfig.FL);
    spawnPlayer();
}

void GameEngine::run()
{
    while (m_running)
    {
        m_entities.update();
        sEnemySpawner();
        sRender();
        sMoviment();
        sUserInput();
        m_currentFrame++;
    }
}
int GameEngine::randNumber(const int max, const int min)
{
    return min + (rand() % (1 + max - min));
}
void GameEngine::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos)
{
    auto e = m_entities.addEntity("bullet");
    // math here
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
}
void GameEngine::sEnemySpawner()
{

    if (m_currentFrame % m_enemyConfig.SI == 0)
    {
        auto enemyVerticesNumber = randNumber(m_enemyConfig.VMAX, m_enemyConfig.VMIN);
        m_lastEnemySpawnTime = m_currentFrame;
        float ex = randNumber(m_window.getSize().x - m_enemyConfig.SR, m_enemyConfig.SR);
        float ey = randNumber(m_window.getSize().y - m_enemyConfig.SR, m_enemyConfig.SR);
        float evx = randNumber(m_enemyConfig.SMAX, m_enemyConfig.SMIN);
        float evy = randNumber(m_enemyConfig.SMAX, m_enemyConfig.SMIN);
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
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
}

void GameEngine::sRender()
{
    m_window.clear();
    for (auto e : m_entities.getEntities())
    {
        e->cShape->circle.setPosition(sf::Vector2(e->CTransform->pos.x, e->CTransform->pos.y));
        e->CTransform->angle += 1.0f;
        e->cShape->circle.setRotation(sf::degrees(e->CTransform->angle));
        m_window.draw(e->cShape->circle);
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

    // player
    m_player->CTransform->velocity = Vec2(0, 0);
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

        e->CTransform->pos.x += e->CTransform->velocity.x;
        e->CTransform->pos.y += e->CTransform->velocity.y;
    }
    for (auto e : m_entities.getEntities("bullet"))
    {
        e->CTransform->pos.x += e->CTransform->velocity.x;
        e->CTransform->pos.y += e->CTransform->velocity.y;
    }
}
