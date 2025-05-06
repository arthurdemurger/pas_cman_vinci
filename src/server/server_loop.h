#ifndef _SERVER_LOOP_H_
#define _SERVER_LOOP_H_

/************************************
 * INCLUDES
 ************************************/
/* Files */
#include "../shared/utils_v3.h"
#include "../game/game.h"
#include "../network/server_network.h"
#include "broadcaster.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * Sets up the SIGINT handler to handle interruptions.
 *
 * PRE:
 *   - None.
 * POST:
 *   - A SIGINT handler is installed to set the `interrupted` flag when SIGINT is received.
 */
void setup_sigint_handler(void);

/**
 * Runs the server loop, accepting clients, launching game components, and resetting after each game.
 *
 * PRE:
 *   - `state` is a valid pointer to an initialized ServerState structure.
 *   - `map_path` is a valid path to the map file.
 * POST:
 *   - The server runs until interrupted, handling multiple game sessions.
 *   - After each game, the server state is reset.
 */
void run_server(ServerState* state, const char* map_path);

#endif // _SERVER_LOOP_H_
