#include <stdio.h>
#include <stdlib.h>
#include "chess.h"
#include "list.h"

t_move_list* new_move_list_element(t_move *m)
{
  t_move_list *ret = calloc(sizeof(t_move_list), 1);
  if (!ret)
    return NULL;
  ret-> move = m;
  ret-> next = NULL;
  return ret;
}

t_move_list* add_move_struct_to_list(t_move_list **lst, t_move *m)
{
  if (*lst == NULL) {
    *lst = new_move_list_element(m);
    if (!*lst)
      return NULL;
    (*lst)->move = m;
    return (*lst);
      
  }
  t_move_list *p = *lst;
  while (p->next)
    p = p->next;
  p->next = new_move_list_element(m);
  if (!p->next)
    return NULL;
  return p->next;
}

t_move_list* add_move_to_list(t_move_list **lst, int start, int end)
{
  t_move *ptr = calloc(sizeof(t_move), 1);
  if (!ptr)
    return NULL;
  ptr->start = start;
  ptr->end = end;
  t_move_list *ret =  add_move_struct_to_list(lst, ptr);
  if (!ret) {
    free(ptr);
    return NULL;
  }
  return ret;
}

int move_list_len(t_move_list *ptr)
{
  if (!ptr)
    return 0;
  int ret = 1;
  while (ptr->next) {
    ret ++;
    ptr = ptr->next;
  }
  return ret;
}

void delete_list(t_move_list *lst)
{
  t_move_list *n;
  while (lst) {
    n = lst->next;
    free(lst->move);
    free(lst);
    lst = n;
  }  
}
