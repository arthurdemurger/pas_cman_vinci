#include "pas_labo.h"

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
