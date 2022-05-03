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

int make_legal_move(int p1, int p2, char* board)
// for the most part this function is the same as just
// moving the piece, but here we are taking in the consideration
// castling and making all the changes on the board
{
  char piece = board[p1];

  //setting all the flags for castling
  if (piece == 'k')
    board[64] |= WHITE_KING_MOVED;
  if (piece == 'K')
    board[64] |= WHITE_KING_MOVED;
  if (piece == 'r' && p1 == 0)
    board[64] |= WHITE_QUEEN_SIDE_ROOK_MOVED;
  if (piece == 'r' && p1 == 7)
    board[64] |= WHITE_KING_SIDE_ROOK_MOVED;
  if (piece == 'R' && p1 == 63)
    board[64] |= BLACK_KING_SIDE_ROOK_MOVED;
  if (piece == 'R' && p1 == 56)
    board[64] |= BLACK_QUEEN_SIDE_ROOK_MOVED;

  //and en-pasaunt
  if (piece == 'p' && p1 / 8 == 1) {
    board[65] = p1 % 8 + 'a';
  }
  else if (piece == 'p' && p1 / 8 == 6) {
    board[65] = p1 % 8 + 'a';
  }
  else {
    board[65] = '\0';
  }
  
  //white king-side castling
  move_piece_on_board_int(p1, p2, board);
  if (p1 == 4 && p2 == 6) {
    move_piece_on_board_int(7, 5, board);
  }
  //white queen-side castling
  else if (p1 == 4 && p2 == 2) {
    move_piece_on_board_int(0, 3, board);
  }
  //black king-side castling
  else if (p1 == 60 && p2 == 62) {
    move_piece_on_board_int(63, 61, board);
  }
  //black queen-side castling
  else if (p1 == 60 && p2 == 58) {
    move_piece_on_board_int(56, 59, board);
  }

  toggle_move(board);
}


void print_board(char *board)
// prints board content
// a1 is on bottom left;
{
  char last_pawn_move = board[65];
  int flags [6];
  flags[0] = board[64] & WHITE_KING_MOVED;
  flags[1] = board[64] & BLACK_KING_MOVED;
  flags[2] = board[64] & WHITE_KING_SIDE_ROOK_MOVED;
  flags[3] = board[64] & WHITE_QUEEN_SIDE_ROOK_MOVED;
  flags[4] = board[64] & BLACK_KING_SIDE_ROOK_MOVED;
  flags[5] = board[64] & BLACK_QUEEN_SIDE_ROOK_MOVED;
  for (int i = 0; i < 6; i ++) {
    if (flags[i] > 0)
      flags[i] = 1;    
  }
  
  //Maybe make something simular to emacs chess
  printf("WKM: %d BKM: %d WKRM: %d WQRM: %d BKRM: %d BQRM: %d\n",
	 flags[0], flags[1], flags[2], flags[3], flags[4], flags[5], flags[6]);
  printf("last pawn move: %c\n", last_pawn_move);
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
