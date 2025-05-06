#ifndef _PAS_CLIENT_H_

#define _PAS_CLIENT_H_

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <string.h>
/* Files */
#include "ui_handler.h"
#include "pas_client_utils.h"
#include "../network/client_network.h"
#include "../shared/utils_v3.h"
#include "../shared/pascman.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * Retrieves the client state.
 *
 * PRE:
 *  - `client_state` is a valid pointer to an initialized ClientState structure.
 * POST:
 * - Returns a pointer to the ClientState structure.
 */
ClientState* get_client_state(void);

#endif // _PAS_CLIENT_H_
