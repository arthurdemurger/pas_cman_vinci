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
#include <stdarg.h>
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
 * Prints a formatted message to the client console.
 *
 * PRE:
 *   - `format` is a valid format string.
 * POST:
 *   - The formatted message is printed to the client console.
 */
void print_client_msg(const char *format, ...);

/**
 * Converts a character to a corresponding direction enum value.
 *
 * PRE:
 *   - `c` is a valid character representing a direction ('<', '>', '^', 'v').
 * POST:
 *   - Returns the corresponding Direction enum value.
 */
enum Direction char_to_direction(char c);


#endif // PAS_CLIENT_UTILS_H
