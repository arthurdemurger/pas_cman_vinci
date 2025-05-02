#include "./pas_server.h"

static ServerState* server_state_ptr = NULL;

// Function to initialize shared memory and reset the game state
struct GameState* initialize_shared_memory(void) {
  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | PERM);
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);
  reset_gamestate(shm_ptr);
  return shm_ptr;
}

// Function to initialize a semaphore
int initialize_semaphore(void) {
  int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | PERM);
  checkNeg(sem_id, "Semget error");

  union semun arg;
  arg.val = 0; // semaphore value 0 -> blocked
  if (semctl(sem_id, 0, SETVAL, arg) == -1) {
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }
  return sem_id;
}

ServerState* get_server_state(void) {
  return server_state_ptr;
}

int main(void) {
  // Initialize shared memory and reset the game state
  struct GameState *shm_ptr = initialize_shared_memory();
  printf("Shared memory initialized.\n");

  // Initialize semaphore
  int sem_id = initialize_semaphore();
  printf("Semaphore initialized.\n");

  // Initialize socket server
  int server_socket = init_socket_server(SERVER_PORT);
  printf("Server started on %s:%d\n", SERVER_IP, SERVER_PORT);

  ServerState state = {0};
  server_state_ptr = &state;
  signal(SIGALRM, timeout_handler);

  // Accept clients
  accept_clients(server_socket, &state);

  // int pipe_fds[2];
  // spipe(pipe_fds); // pipe entre le serveur et le broadcaster

  // FileDescriptor fd_write_broad = pipe_fds[1];
  // FileDescriptor fd_read_broad = pipe_fds[0];

  // Fork le broadcaster
  // pid_t broad_pid = fork_and_run1(run_broadcaster, fd_read_broad);
}
