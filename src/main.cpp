#include "lib/gameEngine/game_engine.h"
#include <SFML/Graphics.hpp>

int main()
{
    GameEngine game("config.txt");
    game.run();
};