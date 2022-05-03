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
  /* play(); */
  char *cb = init_chess_board();
  t_move_list *lst = all_possible_moves(cb);
  print_moves_list(cb, lst);
  return 0;
  /* print_board(cb); */
  /* /\* move_piece_on_board("c1", "f4", cb); *\/ */
  /* int *arr = possible_moves(strpos_to_int("h4"), cb); */
  /* for (int i = 0; arr && arr[i] >= 0 && i < 64; i++) { */
  /*   char *str = int_to_strpos(arr[i]); */
  /*   printf("%s\n", str); */
  /*   free(str); */
  /*   cb[arr[i]] = '*'; */
  /* } */
  /* /\* move_piece_on_board("e7", "e5", cb); *\/ */
  /* /\* printf("%d\n", strpos_to_int("e4")); *\/ */
  /* print_board(cb); */
  /* return 0; */
}
