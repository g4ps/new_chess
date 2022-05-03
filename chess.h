#ifndef EUG_CHESS

#define WHITE_KING_MOVED 0x1
#define WHITE_LEFT_ROOK_MOVED 0x2
#define WHITE_RIGHT_ROOK_MOVED 0x4
#define BLACK_KING_MOVED 0x8
#define BLACK_LEFT_ROOK_MOVED 0x10
#define BLACK_RIGHT_ROOK_MOVED 0x20
#define TURN_COLOR 0x4

int current_turn_color(char *board);
int is_players_piece(char s, int pl);
int power(int base, int pow);
int* bishop_can_move(int p1, char *board);
void toggle_move(char *board);
int* rook_can_move(int p1, char *board);
int* queen_can_move(int p1, char *board);
int* knight_can_move(int p1, char *board);
int *king_can_move(int p1, char *board);
int *pawn_can_move(int p1, char *board);
int *pawn_attacks(int p1, char *board);
int *possible_moves(int p1, char *board);
int move_piece_on_board_int(int p1, int p2, char *board);
int strpos_to_int(char* pos);
char *int_to_strpos(int pos);
int move_piece_on_board(char *p1, char *p2, char *board);
int is_check(char *board);
void print_board(char *board);
char* init_chess_board();
void play();
int is_attacked(int pos, int color, char *board);

#endif
