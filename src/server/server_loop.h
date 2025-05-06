#ifndef _SERVER_LOOP_H_
#define _SERVER_LOOP_H_

#include "../shared/utils_v3.h"
#include "../game/game.h"
#include "../network/server_network.h"
#include "broadcaster.h"

void setup_sigint_handler(void);

void run_server(ServerState* state, const char* map_path);


#endif // _SERVER_LOOP_H_
