//
// Game.cpp
//

#include "pch.h"
#include "game.h"

int WINAPI wWinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PWSTR p_cmd_line, int n_cmd_show)
{
    game game(h_instance);
    if (!game.initialize()) return 0;
    return game.run();
}

game::game(HINSTANCE h_instance) : application(h_instance) {}

game::~game() {}

bool game::initialize()
{
    if (!application::initialize()) return false;
    return true;
}

void game::update(const timer& timer)
{
    
}

void game::render(const timer& timer)
{
    
}



