#ifndef _UI_HANDLER_H_
#define _UI_HANDLER_H_

//************************************
// * INCLUDES
// ************************************/
/* Libraries */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
/* Files */
#include "../shared/utils_v3.h"
#include "../shared/pascman.h"

/**
 * Launches the `pas-cman-ipl` process and sets up pipes for communication.
 *
 * PRE:
 *   - `client_state` is a valid pointer to an initialized ClientState structure.
 * POST:
 *   - The `pas-cman-ipl` process is launched and connected to the client via pipes.
 */
void launch_pas_cman_ipl(ClientState* client_state);

/**
 * Relays commands from the UI process to the server via the socket.
 *
 * PRE:
 *   - `client_state` is a valid pointer to an initialized ClientState structure.
 * POST:
 *   - Commands from the UI process are sent to the server.
 *   - The function terminates when the UI process ends or an error occurs.
 */
void relay_commands(ClientState* client_state);

#endif // _UI_HANDLER_H_
