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
  FILE* file1 = fopen(file_player1, "r");
  FILE* file2 = fopen(file_player2, "r");

  if (!file1 || !file2) {
    if (file1) {
      fclose(file1);
    }
    if (file2) {
      fclose(file2);
    }
    perror("Error opening player files");
    exit(EXIT_FAILURE);
  }

  int c1, c2;
  int end1 = 0, end2 = 0;

  while (!end1 || !end2) {
    if (!end1) {
      c1 = fgetc(file1);
      if (c1 == EOF) {
        end1 = 1;
      } else {
        swrite(pipe1[1], &c1, 1);
      }
    }
    usleep(MOVEMENT_DELAY);
    if (!end2) {
      c2 = fgetc(file2);
      if (c2 == EOF) {
        end2 = 1;
      } else {
        swrite(pipe2[1], &c2, 1);
      }
    }
    usleep(MOVEMENT_DELAY);
  }

  fclose(file1);
  fclose(file2);
  sclose(pipe1[1]);
  sclose(pipe2[1]);
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

  usleep(SERVER_STARTUP_DELAY);

  client1_pid = start_client(port, pipe1);
  usleep(CLIENT_STARTUP_DELAY);
  client2_pid = start_client(port, pipe2);

  process_movements(player1_file, player2_file, pipe1, pipe2);

  printf("Wait 5 seconds to see the end of the game\n");
  sleep(END_GAME_WAIT_SEC);

  // Terminate the server and clients
  printf("Terminating server and clients...\n");
  if (server_pid > 0) kill(server_pid, SIGINT);
  if (client1_pid > 0) kill(client1_pid, SIGTERM);
  if (client2_pid > 0) kill(client2_pid, SIGTERM);

  swaitpid(server_pid, NULL, 0);
  swaitpid(client1_pid, NULL, 0);
  swaitpid(client2_pid, NULL, 0);
  printf("Server and clients terminated.\n");

  return EXIT_SUCCESS;
}
