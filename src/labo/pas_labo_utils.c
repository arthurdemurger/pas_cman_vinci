#include "pas_labo_utils.h"

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

pid_t start_client(const char* port, int pipe_fd[2]) {
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

void process_movements(char* file_player1, char* file_player2, int pipe1[2], int pipe2[2]) {
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
