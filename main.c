#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "chess.h"
#include "list.h"

int main()
{
  play();
  char *cb = init_chess_board();
  t_move_list *lst = all_possible_moves(cb);

  print_board(cb);
  print_moves_list(cb, lst);
  
  printf(" material: %f\n", dumb_eval(cb, 3));
  delete_list(lst);
  free(cb);
  return 0;
}
