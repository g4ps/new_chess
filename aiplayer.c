#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "chess.h"
#include "list.h"

float material_eval(char *board)
{
  float ret = 0;
  for (int i = 0; i < 64; i++) {
    if (!isalpha(board[i]))
      continue;
    char piece = tolower(board[i]);
    float piece_val = 0;
    switch(piece) {
    case 'r':
      piece_val = 5;
      break;
    case 'n':
      piece_val = 3;
      break;
    case 'b':
      piece_val = 3;
      break;
    case 'q':
      piece_val = 9;
      break;
    case 'p':
      piece_val = 1;
      break;
    default:
      piece_val = 0;
      break;
    }
    if (isupper(board[i]))
      piece_val *= -1;
    ret += piece_val;
  }
  return ret;
}

float dumb_eval(char *board, int max_depth)
{
  //getting a couple of current stats
  float curr_mat_eval = material_eval(board);
  int turn_color = current_turn_color(board);
  
  //if checkmate, then we return some very large value
  if (is_checkmate(board)) {
    if (turn_color)
      return -1000;
    return 1000;
  }

  //if we have reached maximum depth, then we should leave
  //and return current evaluation
  if (max_depth <= 0)
    return curr_mat_eval;

  //getting all the moves
  t_move_list *move_list = all_possible_moves(board);
  if (move_list == NULL)
    return 0;

  //we need a variable for best evaluation
  float move_eval;
  float best_eval = 0;
  int init = 1;

  //iterating through all moves;
  for (t_move_list *st = move_list; st; st = st->next) {

    //making a move
    char *temp_board = get_board_copy(board);
    if (!temp_board)
      return 0;
    make_legal_move(st->move->start, st->move->end, temp_board);
    toggle_move(temp_board);
    
    move_eval = dumb_eval(temp_board, max_depth - 1);

    //depending on the player, the best value is either highest
    // or the lowest
    if (turn_color == 0) {
      if (best_eval < move_eval || init)
	best_eval = move_eval;      
    }
    else {
      if (best_eval > move_eval || init)
	best_eval = move_eval;
    }
    init = 0; //init is used for first setting of the best_eval
    free(temp_board);
  }
  delete_list(move_list);
  return best_eval;
}

t_move dumb_get_move(char *board)
{
  int turn_color = current_turn_color(board);

  int max_depth = 3;

  
  t_move ret = {-1, -1};
  
  t_move_list *move_list = all_possible_moves(board);
  t_move_list *st;
  if (!move_list)
    return ret;

  float max_eval;
  int init = 1;  
  for (st = move_list; st; st = st->next) {    
    char *temp_board = get_board_copy(board);
    if (!temp_board)
      return ret;
    
    make_legal_move(st->move->start, st->move->end, temp_board);
    toggle_move(board);
    
    float curr_eval = dumb_eval(temp_board, max_depth);
    free(temp_board);
    if (turn_color == 0) {
      if (curr_eval < max_eval || init) {
	max_eval = curr_eval;
	ret = *(st->move);
      }
    }
    else {
      if (curr_eval > max_eval || init) {
	max_eval = curr_eval;
	ret = *(st->move);
      }
    }
    init = 0;    
  }
  delete_list(move_list);
  return ret;
}
