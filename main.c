#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "chess.h"
#include "list.h"
#include <time.h>
#include <fcntl.h>

const int ENABLE_LOGGING = 1;

int log_file()
{
  char *str = "log_file_";
  char rand_str[6];
  char *alpha = "qwertyuiopasdfghjklzxcvbnm1234567890";
  for (int i = 0; i < 5; i++) {
    rand_str[i] = alpha[rand() % strlen(alpha)];
  }
  rand_str[5] = '\0';
  char log_file_name[20];
  strcpy(log_file_name, "logs/");
  strcat(log_file_name, str);
  strcat(log_file_name, rand_str);
  printf("log file is %s\n", log_file_name);
  return open(log_file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
}

char *get_starting_position_from_file(char *filename)
{
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror(filename);
    exit(1);
  }
  char *buf = calloc(sizeof(char), 1024);
  read(fd, buf, 1024);
  close(fd);
  char *ret = calloc(sizeof(char), 70);  
  int buf_pos = 0;
  int board_pos = 0;
  while (buf_pos < 1024 && board_pos < 64) {
    if (strchr("rnbkqpRNBKQP", buf[buf_pos]))
      ret[board_pos++] = buf[buf_pos];
    else if (buf[buf_pos] != '\n')
      ret[board_pos++] = '_';
    buf_pos++;
  }
  free(buf);
  if (board_pos == 64)
    return ret;
  free(ret);
  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  int log_fd = -1;
  if (ENABLE_LOGGING) {
    log_fd = log_file();
    if (log_fd < 0) {
      printf("Couldn't openn log file:(\n");
      exit(1);
    }
  }
  char *starting_pos = NULL;
  if (argc == 2)
    starting_pos = get_starting_position_from_file(argv[1]);
  play(log_fd, starting_pos);
  return 0;
  char *cb = starting_pos;
  if (!cb)
    cb = init_chess_board();
  t_move_list *lst = all_possible_moves(cb);

  print_board(cb);
  print_moves_list(cb, lst);
  
  printf(" material: %f\n", activity_eval(cb));
  delete_list(lst);
  free(cb);
  return 0;
}
