#ifndef PAS_CLIENT_UTILS_H
#define PAS_CLIENT_UTILS_H

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Files */
#include "../shared/utils_v3.h"
#include "pas_client.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * Cleans up resources by closing file descriptors and terminating child processes.
 *
 * PRE:
 *   - `client_state` is a valid pointer to an initialized ClientState structure.
 * POST:
 *   - All resources associated with the client are released.
 */
void cleanup(ClientState *client_state);

/**
 * Sets up signal handlers for specified signals.
 *
 * POST:
 *   - Signal handlers for SIGINT and SIGTERM are registered.
 */
void setup_signal_handlers(void);

#endif // PAS_CLIENT_UTILS_H
