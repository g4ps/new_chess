#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "chess.h"


int current_turn_color(char *board)
{
  if (!(board[64] & TURN_COLOR))
    return 0;
  return 1;
}

int is_players_piece(char s, int pl)
{
  if (islower(s) && pl == 0)
    return 1;
  if (isupper(s) && pl == 1)
    return 1;
  return 0;
}

int power(int base, int pow)
{
  int ret = 1;
  for (int i = 0; i < pow; i++) {
    ret *= base;
  }
  return ret;
}

void toggle_move(char *board)
{
  if (!(board[64] & TURN_COLOR))
    board[64] |= TURN_COLOR;
  else
    board[64] &= 0xff ^ TURN_COLOR;
}

int move_piece_on_board_int(int p1, int p2, char *board)
//swaps contents of two squares on the board
{
  if (p1 >= 64 || p2 >= 64 || p1 < 0 || p2 < 0) //if out of bounds
    return -1;
  char piece = board[p1];
  if (!strchr("prnbqkPRNBKQ", piece)) { //no piece is selected
    return -1;
  }
  board[p2] = board[p1];
  board[p1] = '_';
  return 0;  
}


int strpos_to_int(char* pos)
// converts square into an integer
// i.e. e4 -> 28;
{
  char col = pos[0];
  char row = pos[1];
  if (!strchr("abcdefgh", col))
    return -1;
  if (!strchr("12345678", row))
    return -1;
  return (col - 'a') + (row - '1') * 8;
}

char *int_to_strpos(int pos)
//converts integer position into string
{
  char *ret = calloc(sizeof(char), 3);
  ret[0] = pos % 8 + 'a';
  ret[1] = pos / 8 + '1';
  return ret;
}

int move_piece_on_board(char *p1, char *p2, char *board)
// same as move_piece_on_board_int for string format moves such as g4 or e4
{
  return move_piece_on_board_int(strpos_to_int(p1), strpos_to_int(p2), board);
}


void print_board(char *board)
// prints board content
// a1 is on bottom left;
{
  //Maybe make something simular to emacs chess
  printf("  a b c d e f g h \n");
  for (int i = 7; i >= 0; i--) {
    printf("%d ", i + 1);
    for (int j = 0; j < 8; j++) {
      printf("%c ", board[i * 8 + j]);
    }
    printf("%d ", i + 1);
    printf("\n");
  }
  printf("  a b c d e f g h \n");
}
