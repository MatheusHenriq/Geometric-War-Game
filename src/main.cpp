#include "lib/gameEngine/game_engine.h"

int main(int argc, char *argv[])
{
    GameEngine game = GameEngine("config.txt");
    game.run();
    return 0;
};