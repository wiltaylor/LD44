#include "gamemanager.h"

static bool quiting = false;

void quit_game()
{
    quiting = true;
}

bool is_quiting()
{
    return quiting;
}
