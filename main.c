#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


const char WHITE_KING_MOVED = 0x1;
const char BLACK_KING_MOVED = 0x2;
const char TURN_COLOR = 0x4;

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



void toggle_move(char *board)
{
  if (!(board[64] & TURN_COLOR))
    board[64] |= TURN_COLOR;
  else
    board[64] &= 0xff ^ TURN_COLOR;
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
      else if ((color == 0 && p1 / 8 == 1 && i == 1) ||
               (color == 1 && p1 / 8 == 6 && i == 1) && board[temp] == '_' && board[temp + (color * 2 - 1) * 8] == '_')
        ret[pos++] = temp;
      else if (board[temp] == '_' && i == 0)
        ret[pos++] = temp;
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

int main()
{
  play();
  char *cb = init_chess_board();
  print_board(cb);
  /* move_piece_on_board("c1", "f4", cb); */
  int *arr = possible_moves(strpos_to_int("h4"), cb);
  for (int i = 0; arr && arr[i] >= 0 && i < 64; i++) {
    char *str = int_to_strpos(arr[i]);
    printf("%s\n", str);
    free(str);
    cb[arr[i]] = '*';
  }
  /* move_piece_on_board("e7", "e5", cb); */
  /* printf("%d\n", strpos_to_int("e4")); */
  print_board(cb);
  return 0;
}
