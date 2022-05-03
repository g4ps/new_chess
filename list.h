#ifndef MOVELIST
#define MOVELIST

typedef struct move
{
  int start;
  int end;  
} t_move;

typedef struct move_list
{
  t_move *move;
  struct move_list *next;
} t_move_list;

t_move_list* new_move_list_element(t_move *m);
t_move_list* add_move_struct_to_list(t_move_list **lst, t_move *m);
t_move_list* add_move_to_list(t_move_list **lst, int start, int end);
int move_list_len(t_move_list *ptr);
void delete_list(t_move_list *lst);

#endif
