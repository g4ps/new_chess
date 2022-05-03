#include "chess.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int* bishop_can_move(int p1, char *board)
//returns an array of cells where bishop can move
//from a given position; includes first juicer in the path no
//matter the color;
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;  
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  /* if (!strchr("Bb", board[p1])) //if the cell is not a bishop */
  /*   return NULL; */
  int dir[] = {1, 1, 1, 1}; // directions in which bishop can move
  //tunrs into 0 when an obstacle is found;
  //top left, top right, bottom left, bottom right
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  for (int i = 0; i <= 8; i++) {
    int temp = 0;
    for (int j = 0; j < 4; j++) {
      if (dir[j]) {
        //calculating delta from current position        
        temp = (7 + (j % 2) * 2) *  i;
        //adding or substructing from current positon depending on the direction
        if (j < 2)
          temp = p1 + temp;
        else
          temp = p1 - temp;
        if (temp < 64 && temp >= 0) {
          if (temp != p1)
            if (board[temp] == '_' || //testing those two lines
                !is_players_piece(board[temp], color))
              ret[pos++] = temp;
          if (temp != p1 && board[temp] != '_')
            dir[j] = 0;
          //not overshooting on left-right sides of the board
          if ((j == 0 || j == 3) && temp % 8 == 0)
            dir[j] = 0;
          if ((j == 1 || j == 2) && temp % 8 == 7)
            dir[j] = 0;
        }
        else
          dir[j] = 0;
      }
    }
  }
  return ret;
}


int* rook_can_move(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;  
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  /* if (!strchr("Rr", board[p1])) //if the cell is not a rook */
  /*   return NULL; */
  int dir[] = {1, 1, 1, 1}; // directions in which rook can move
  //tunrs into 0 when an obstacle is found;
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  for (int i = 0; i < 8; i++) {
    int temp = 0;
    for (int j = 0; j < 4; j++) {
      if (dir[j]) {
        if (j < 2) {
          temp = p1 + (j * 2 - 1) * 8 * i;
        }
        else
          temp = p1 + ((j - 2) * 2 - 1) * i;
        if (temp < 64 && temp >= 0) {
          if (temp != p1)
            if (board[temp] == '_' ||
                !is_players_piece(board[temp], color))
              ret[pos++] = temp;
          if (temp != p1 && board[temp] != '_') {
            dir[j] = 0;
          }
          //not overshooting on left-right sides of the board
          if (j == 2 && temp % 8 == 0)
            dir[j] = 0;
          if (j == 3 && temp % 8 == 7)
            dir[j] = 0;
        }
        else
          dir[j] = 0;
      }
    }
  }
  return ret;
}

int* queen_can_move(int p1, char *board)
{
  int *bishop_moves = bishop_can_move(p1, board);
  int *rook_moves = rook_can_move(p1, board);
  if (bishop_moves == NULL || rook_moves == NULL) {
    free(bishop_moves);
    free(rook_moves);
    return NULL;
  }
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;  
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  for (int i = 0; bishop_moves[i] >= 0 && i < 64; i++) {
    ret[pos++] = bishop_moves[i];
  }
  for (int i = 0; rook_moves[i] >= 0 && i < 64; i++) {
    ret[pos++] = rook_moves[i];
  }
  free(bishop_moves);
  free(rook_moves);
  return ret;
}

int* knight_can_move(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  /* if (!strchr("Bb", board[p1])) //if the cell is not a bishop */
  /*   return NULL; */
  int moves[] = {6, 15, 17, 10}; // all posible knight moves deltas;
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  for (int i = 0; i < 8; i++) {
    int temp = moves[i % 4];
    if (i >= 4)
      temp = p1 - temp;
    else
      temp = p1 + temp;
    /* temp = p1 - 15; */
    if (temp >= 0 && temp < 64)
      if (!
          (
           (p1 % 8 == 1 && (i == 0 || i == 7)) ||
           (p1 % 8 == 0 && (i < 2 || i > 5)) ||
           (p1 % 8 == 6 && (i == 3 || i == 4)) ||
           (p1 % 8 == 7 && (i >= 2 && i <= 5)) 
            ))
        if (board[temp] == '_' ||
            !is_players_piece(board[temp], color))
          ret[pos++] = temp;
  }
  return ret;
}

int *king_can_move(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  for (int i = 0; i < 9; i++) {
    int temp = p1 + (i / 3 - 1) * 8 + i % 3 - 1;
    if (temp >= 0 && temp < 64)
      if (!(
            (p1 % 8 == 0 && i % 3 == 0) ||
            (p1 % 8 == 7 && i % 3 == 2)) && temp != p1)
        if (board[temp] == '_' ||
            !is_players_piece(board[temp], color))
          ret[pos++] = temp;
  }

  //handling castling

  //for white
  if (color == 0 && p1 / 8 == 0 && !(board[64] & WHITE_KING_MOVED)&& ! is_attacked(strpos_to_int("e1"), !color, board)) {
    if (board[strpos_to_int("f1")] == '_' &&
        board[strpos_to_int("g1")] == '_' &&
        ! is_attacked(strpos_to_int("f1"), !color, board) &&
        ! is_attacked(strpos_to_int("g1"), !color, board) &&
        ! (board[64] & WHITE_KING_SIDE_ROOK_MOVED)
        ) {
      ret[pos++] = strpos_to_int("g1");
    }
    if (board[strpos_to_int("d1")] == '_' &&
        board[strpos_to_int("c1")] == '_' &&
        ! is_attacked(strpos_to_int("d1"), !color, board) &&
        ! is_attacked(strpos_to_int("c1"), !color, board) &&
        ! (board[64] & WHITE_QUEEN_SIDE_ROOK_MOVED)
        ) {
      ret[pos++] = strpos_to_int("c1");
    }
  }

  //for black
  if (color == 1 && p1 / 8 == 7 && !(board[64] & BLACK_KING_MOVED)&& ! is_attacked(strpos_to_int("e8"), !color, board)) {
    if (board[strpos_to_int("f8")] == '_' &&
        board[strpos_to_int("g8")] == '_' &&
        ! is_attacked(strpos_to_int("f8"), !color, board) &&
        ! is_attacked(strpos_to_int("g8"), !color, board) &&
        ! (board[64] & BLACK_KING_SIDE_ROOK_MOVED)
        ) {
      ret[pos++] = strpos_to_int("g8");
    }
    if (board[strpos_to_int("d8")] == '_' &&
        board[strpos_to_int("c8")] == '_' &&
        ! is_attacked(strpos_to_int("d8"), !color, board) &&
        ! is_attacked(strpos_to_int("c8"), !color, board) &&
        ! (board[64] & BLACK_QUEEN_SIDE_ROOK_MOVED)
        ) {
      ret[pos++] = strpos_to_int("c8");
    }
  }
  
  return ret;
}

int *king_attacks(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  for (int i = 0; i < 9; i++) {
    int temp = p1 + (i / 3 - 1) * 8 + i % 3 - 1;
    if (temp >= 0 && temp < 64)
      if (!(
            (p1 % 8 == 0 && i % 3 == 0) ||
            (p1 % 8 == 7 && i % 3 == 2)) && temp != p1)
        if (board[temp] == '_' ||
            !is_players_piece(board[temp], color))
          ret[pos++] = temp;
  }
  
  return ret;
}

int *pawn_can_move(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  char c = board[p1];
  if (isupper(c))
    c = 0;
  else
    c = 1;
  int moves[] = {8, 16, 7, 9};
  int turn_color = current_turn_color(board);
  int color = 0;
  if (isupper(board[p1]))
    color = 1;
  for (int i = 0; i < 4; i++) {
    int temp = p1 + (c * 2 - 1) * moves[i];
    if (!(
          (p1 % 8 == 0 && i == 3 && c == 0) ||
          (p1 % 8 == 7 && i == 2 && c == 0) ||
          (p1 % 8 == 0 && i == 2 && c == 1) ||
          (p1 % 8 == 7 && i == 3 && c == 1)
          )
        ) {
      if (i >= 2 && (strchr("rnbqkp", board[temp]) && color == 1 || strchr("RNBQKP", board[temp]) && color == 0))
        ret[pos++] = temp;
      else if (((color == 0 && p1 / 8 == 1 && i == 1) ||
               (color == 1 && p1 / 8 == 6 && i == 1)) && board[temp] == '_' && board[p1 + (color * -2 + 1) * 8] == '_')
        ret[pos++] = temp;
      else if (board[temp] == '_' && i == 0)
        ret[pos++] = temp;
    }
  }

  //handling en-passaunt
  if ((color == 0 && p1 / 8 == 4)
      || (color == 1 && p1 / 8 == 3)) {  // if the pawn is on correct row
    if (p1 % 8 == board[65] - 'a' + 1 && p1 % 8 != 0) { // if the pawn is on the left
      ret[pos++] = p1 + 7 * ((color * -2) + 1);
    }
    else if (p1 % 8 == board[65] - 'a' - 1 && p1 % 8 != 7) {// and on the right
      ret[pos++] = p1 + 9 * ((color * -2) + 1);
    }
  }

  
  return ret;
}

int *pawn_attacks(int p1, char *board)
{
  int *ret = calloc(64, sizeof(int));
  if (!ret)
    return NULL;
  for (int i = 0; i < 64; i++) {
    ret[i] = -1;    
  }
  int pos = 0;
  if (p1 >= 64 || p1 < 0) //if out of bounds
    return NULL;
  char c = board[p1];
  if (isupper(c))
    c = 0;
  else
    c = 1;
  int moves[] = {8, 16, 7, 9};  
  for (int i = 0; i < 4; i++) {
    int temp = p1 + (c * 2 - 1) * moves[i];
    if (!(
          (p1 % 8 == 0 && i == 3 && c == 0) ||
          (p1 % 8 == 7 && i == 2 && c == 0) ||
          (p1 % 8 == 0 && i == 2 && c == 1) ||
          (p1 % 8 == 7 && i == 3 && c == 1)
          )
        )
      if (i >= 2)
        ret[pos++] = temp;
  }
  return ret;
}
