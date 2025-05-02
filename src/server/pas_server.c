#include "./pas_server.h"

#define PERM 0666
#define MAX_PLAYERS 2
#define BACKLOG 2
#define SERVER_IP "127.0.0.1" // localhost
#define SERVER_PORT 15235

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

int initSocketServer(int serverPort)
{
  int sockfd = ssocket();

  /* no socket error */
  sbind(serverPort, sockfd);

  /* no bind error */
  slisten(sockfd, BACKLOG);

  /* no listen error */
  return sockfd;
}

void timeout_handler(int sig) {
  if (server_state_ptr && server_state_ptr->clients_connected == 1) {
    close(server_state_ptr->client_sockets[0]);
    printf("Client 1 disconnected due to timeout.\n");
    server_state_ptr->clients_connected = 0;
  }
}

// Function to handle a single client connection
void handle_client(int client_sock, int server_socket) {
  close(server_socket);
  // Add client-specific handling logic here
  exit(0);
}

// Function to accept client connections and manage the server state
void accept_clients(int server_socket, ServerState *state) {
  while (state->clients_connected < MAX_PLAYERS) {
    if (state->clients_connected == 1) {
      alarm(30); // Set a timeout for the second client
    }

    int client_sock = saccept(server_socket);
    alarm(0); // Cancel the timeout once a client connects

    printf("Client %d connected.\n", state->clients_connected + 1);
    state->client_sockets[state->clients_connected] = client_sock;

    pid_t pid = fork();
    if (pid == 0) {
      handle_client(client_sock, server_socket);
    }

    state->clients_connected++;
  }
}

int main(void) {
  // Initialize shared memory and reset the game state
  struct GameState *shm_ptr = initialize_shared_memory();
  printf("Shared memory initialized.\n");

  // Initialize semaphore
  int sem_id = initialize_semaphore();
  printf("Semaphore initialized.\n");

  // Initialize socket server
  int server_socket = initSocketServer(SERVER_PORT);
  printf("Server started on %s:%d\n", SERVER_IP, SERVER_PORT);

  ServerState state = {0};
  server_state_ptr = &state;
  signal(SIGALRM, timeout_handler);

  // Accept clients
  accept_clients(server_socket, &state);
}
