#ifndef _PAS_LABO_UTILS_H_
#define _PAS_LABO_UTILS_H_

/************************************
 * CONSTANTS
 ************************************/
#define MOVEMENT_DELAY 100000

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
/* Files */
#include "../shared/utils_v3.h"

/**
 * Prints a formatted message to the standard output with a specific prefix.
 *
 * PRE:
 *  - `format` is a valid format string.
 *
 * POST:
 * - The formatted message is printed to the standard output with a prefix "[LABO]".
 *
 */
void print_labo_msg(const char *format, ...);

/**
 * Starts a client process.
 *
 * PRE:
 *   - `port` is a valid string representing the server port.
 *   - `pipe_fd` is either NULL or a valid pipe file descriptor array of size 2.
 * POST:
 *   - A new client process is created and connected to the server.
 * RES:
 *   - Returns the PID of the created client process.
 */
pid_t start_client(const char* port, int pipe_fd[2]);

/**
 * Processes movements from two player files and sends them to the respective pipes.
 *
 * PRE:
 *   - `file_player1` and `file_player2` are valid paths to readable files.
 *   - `pipe1` and `pipe2` are valid pipe file descriptor arrays of size 2.
 * POST:
 *   - The contents of `file_player1` are sent to `pipe1`.
 *   - The contents of `file_player2` are sent to `pipe2`.
 *   - Both pipes are closed after processing.
 * RES:
 *   - None.
 */
void process_movements(char* file_player1, char* file_player2, int pipe1[2], int pipe2[2]);


#endif // _PAS_LABO_UTILS_H_
