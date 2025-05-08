#ifndef _PAS_SERVER_H_
#define _PAS_SERVER_H_

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/* Files */
#include "ipc.h"
#include "broadcaster.h"
#include "server_utils.h"
#include "../game/client_handler.h"
#include "../game/game.h"
#include "../network/server_network.h"
#include "../shared/utils_v3.h"
#include "../ui/student_kit/pascman.h"

/************************************\
 * FUNCTIONS
\************************************/
/**
 * PRE:  None
 * POST: Returns a pointer to the current ServerState structure.
 *       If the ServerState is not initialized, returns NULL.
 * RES:  Pointer to the ServerState structure or NULL if uninitialized.
 */
ServerState* get_server_state(void);

#endif // _PAS_SERVER_H_
