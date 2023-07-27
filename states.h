#ifndef _STATES_
#define _STATES_

#include "jewel.h"
#include "structs.h"

void state_begin(game_t* game);

void state_help(game_t* game);

void state_score(game_t* game);

void state_select(game_t* game);

void state_changing(jewel_t first, jewel_t second, game_t* game);

#endif
