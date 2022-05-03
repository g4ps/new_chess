#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "chess.h"


int *possible_moves(int p1, char *board)
{
  char square = tolower(board[p1]);
  int current_color = current_turn_color(board);
  if (square == '_')
    return NULL;
  if (
      /* (current_color == 0 && strchr("PRNBQK", square)) || */
      (strchr("prnbqk", square))
      ) {
      int* ret;
      switch(square) {
      case 'p':
        ret = pawn_can_move(p1, board);
        break;
      case 'r':
        ret = rook_can_move(p1, board);
        break;
      case 'n':
        ret = knight_can_move(p1, board);
        break;
      case 'b':
        ret = bishop_can_move(p1, board);
        break;
      case 'q':
        ret = queen_can_move(p1, board);
        break;
      case 'k':
        ret = king_can_move(p1, board);
        break;
      default:
        break;
      }
      return ret;
  }
  return NULL;
}

int is_check(char *board)
{
  int current_color = current_turn_color(board);
  char king_char = 'k';
  if (current_color == 1)
    king_char = 'K';
  int king_pos = strchr(board, king_char) - board;
  for (int i = 0; i < 64; i++) {
    char square = board[i];
    if (square == '_')
      continue;
    if ((current_color == 0 && strchr("PRNBQK", square)) ||
        (current_color == 1 && strchr("prnbqk", square))
        ) {
      square = tolower(square);
      int* possible_moves;
      switch(square) {
      case 'p':
        possible_moves = pawn_attacks(i, board);
        break;
      case 'r':
        possible_moves = rook_can_move(i, board);
        break;
      case 'n':
        possible_moves = knight_can_move(i, board);
        break;
      case 'b':
        possible_moves = bishop_can_move(i, board);
        break;
      case 'q':
        possible_moves = queen_can_move(i, board);
        break;
      case 'k':
        possible_moves = king_can_move(i, board);
        break;
      default:
        break;
      }

      for (int i = 0; i < 64 && possible_moves
             && possible_moves[i] >= 0; i++) {
        if (king_pos == possible_moves[i])
          return 1;
      }
      free(possible_moves);
    }
  }
  return 0;
}

char* init_chess_board()
{  
  char* ret = calloc(sizeof(char), 66);
  if (!ret)
    return NULL;

  
  /* char *inboard  = */
  /*   "rnbqkbnr" */
  /*   "pppppppp" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "PPPPPPPP" */
  /*   "RNBQKBNR"; */

  /* char *inboard  = */
  /*   "_____b__" */
  /*   "________" */
  /*   "________" */
  /*   "____n___" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "__B__B__"; */
  /* char *inboard  = */
  /*   "rnbqkbnr" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "________" */
  /*   "RNBQKBNR"; */

  
  char *inboard  =
    "rnbqkbnr"
    "_pppp__p"
    "p____p__"
    "______pQ"
    "________"
    "____P___"
    "PPPP_PPP"
    "RNB_KBNR";
  
  /* char *inboard  = */
  /*   "rn_qkbnr" */
  /*   "pppppppp" */
  /*   "__n_____" */
  /*   "____b___" */
  /*   "___Q____" */
  /*   "________" */
  /*   "PPPPPPPP" */
  /*   "RNBQKBNR"; */
  strcpy(ret, inboard);
  ret[64] = '\0';
  return ret;
  
  char* white = "rnbqkbnrpppppppp";
  char* black = "PPPPPPPPRNBQKBNR";
    
  for (int i = 0; i < 64; i++) {
    char square;
    if (i < 16)
      square = white[i];
    else if (i >= 64 - 16)
      square = black[i - (64 - 16)];
    else
      square = ' ';
    ret[i] = square;
  }
  ret[64] = ' ';
  return ret;
}


void play()
{
  char *board = init_chess_board();
  while(1) {
    //Printing color of the current player
    int turn_color = current_turn_color(board);
    if (turn_color == 0)
      printf("WHITE\n");
    else
      printf("BLACK\n");
    if (is_check(board)) {
      printf("CHECK!\n");
    }
    print_board(board);

    // reading move in form "e4 e5"
    char *start = "";
    do {
      start = readline("> ");
      if (start == NULL)
        exit(0);
    } while (strlen(start) < 5);

    //Morphing moves into positions
    int p1 = strpos_to_int(start);    
    int p2 = strpos_to_int(start + 3);
    if (p1 < 0 || p2 < 0)
      continue;

    //Making sure there is a piece on starting position
    if (!isalpha(board[p1]))
      continue;
    //Making sure, that the piece is of correct color
    if (!isupper(board[p1]) != !turn_color)
      continue;

    //Looking which move is possible with current piece
    int *moves = possible_moves(p1, board);
    printf("Possible moves: ");
    int is_possible = 0;
    for (int i = 0; i < 64 && moves[i] >= 0; i++) {
      char *str = int_to_strpos(moves[i]);      
      printf("%s ", str);
      free(str);
      if (moves[i] == p2)
        is_possible = 1;
    }
    printf("\n");
    free(moves);

    if (is_possible)
      move_piece_on_board_int(p1, p2, board);
    else
      continue;
    
    toggle_move(board);
  }
}