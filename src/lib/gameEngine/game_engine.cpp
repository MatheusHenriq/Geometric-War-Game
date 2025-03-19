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
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S;
        }
        else if (type == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
        }
        else if (type == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.S;
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
    //    spawnPlayer();
}

void GameEngine::run()
{
    while (m_running)
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                {
                    m_running = false;
                    m_window.close();
                }
            }

            m_window.clear();
            m_window.display();
            //  m_entities.update();
            m_currentFrame++;
        }
    }
}