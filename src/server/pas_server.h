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

/**
 * PRE:  format: a format string for the message to be printed.
 *      ...: additional arguments for the format string.
 * POST: Prints a formatted message to the server console.
 *      The message is prefixed with "[SERVER] ".
 */
void print_server_msg(const char *format, ...);

#endif // _PAS_SERVER_H_
