#include "pas_labo.h"

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
static pid_t start_client(const char* port, int pipe_fd[2]);

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
static void process_movements(char* file_player1, char* file_player2, int pipe1[2], int pipe2[2]);

void print_labo_msg(const char *format, ...) {
  va_list args;
  va_start(args, format);
  colorOn(1, RED_TEXT);
  printf(" [LABO]  ");
  colorOff();
  vprintf(format, args);
  printf("\n");
  va_end(args);
}


static pid_t start_client(const char* port, int pipe_fd[2]) {
  pid_t pid = sfork();

    if (pid == 0) {
        if (pipe_fd != NULL) {
            sdup2(pipe_fd[0], STDIN_FILENO);
            sclose(pipe_fd[0]);
            sclose(pipe_fd[1]);
        }
        sexecl("./build/pas_client", "pas_client", "localhost", port, "-test", NULL);
    }
    if (pipe_fd != NULL) {
        sclose(pipe_fd[0]);
    }

    return pid;
}

static void process_movements(char* file_player1, char* file_player2, int pipe1[2], int pipe2[2]) {
  FileDescriptor file1 = sopen(file_player1, O_RDONLY, 0);
  FileDescriptor file2 = sopen(file_player2, O_RDONLY, 0);

  ssize_t n;
  char dir;
  int end1 = 0, end2 = 0;

  while (!end1 || !end2) {
    if (!end1) {
      n = sread(file1, &dir, sizeof(char));
      if (!n) {
        swrite(pipe1[1], "\0", sizeof(char));
        end1 = 1;
      } else {
        swrite(pipe1[1], &dir, sizeof(char));
      }
    }
    usleep(MOVEMENT_DELAY);
    if (!end2) {
      n = sread(file2, &dir, sizeof(char));
      if (!n) {
        swrite(pipe2[1], "\0", sizeof(char));
        end2 = 1;
      } else {
        swrite(pipe2[1], &dir, sizeof(char));
      }
    }
    usleep(MOVEMENT_DELAY);
  }

  close(file1);
  close(file2);
  sclose(pipe1[1]);
  sclose(pipe2[1]);

  print_labo_msg("Finished processing movements");
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
      fprintf(stderr, "Usage: %s <port> <map.txt> <joueur1.txt> <joueur2.txt>\n", argv[0]);
      return EXIT_FAILURE;
  }
  pid_t server_pid, client1_pid, client2_pid;
  char* port = argv[1];
  char* map_path = argv[2];
  char* player1_file = argv[3];
  char* player2_file = argv[4];

  int pipe1[2], pipe2[2];
  spipe(pipe1);
  spipe(pipe2);

  server_pid = sfork();
  if (server_pid == 0) {
    sclose(pipe1[0]);
    sclose(pipe2[0]);
    sclose(pipe1[1]);
    sclose(pipe2[1]);
    sexecl("./build/pas_server", "pas_server", port, map_path, NULL);
  }
  print_labo_msg("Server started");

  usleep(SERVER_STARTUP_DELAY);

  client1_pid = start_client(port, pipe1);
  print_labo_msg("Client [1] started");

  usleep(CLIENT_STARTUP_DELAY);

  client2_pid = start_client(port, pipe2);
  print_labo_msg("Client [2] started");

  process_movements(player1_file, player2_file, pipe1, pipe2);

  sleep(END_GAME_WAIT_SEC);

  // Terminate the server and clients
  print_labo_msg("Terminating server and clients...");
  if (server_pid > 0) {
    kill(server_pid, SIGINT);
  }

  swaitpid(server_pid, NULL, 0);
  swaitpid(client1_pid, NULL, 0);
  swaitpid(client2_pid, NULL, 0);

  print_labo_msg("Server and clients terminated");

  return EXIT_SUCCESS;
}
