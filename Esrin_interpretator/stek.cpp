/*
  License GPL V3.
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
  program to implement stek.
  Later debagger code will be added here also.
*/

#include "esrin.h"

struct stek fnstk, obstk, dovrstk, secstk, glbstk, valstk, adrstk;


void init_stek(){

  fnstk.top = NULL;
  obstk.top = NULL;
  dovrstk.top = NULL;
  secstk.top = NULL;
  glbstk.top = NULL;
  valstk.top = NULL;
  adrstk.top = NULL;

};

void push_to_stek(struct stek *stk, void *dptr){

  struct stek_el *tmp = (struct stek_el *)malloc(sizeof(struct stek_el));
  tmp->dptr = dptr;
  tmp->prev = stk->top;
  stk->top = tmp;
}

void *pop_from_stek(struct stek *stk){

  struct stek_el *tmp;
  void *dptr;

  if (stk->top == NULL)
    return NULL; /* stek boshdu  */
  
  dptr = stk->top->dptr;
  tmp = stk->top;
  stk->top = stk->top->prev;
  tmp->prev = NULL;
  free(tmp);

  return dptr;
}


#define stktop(stk) pop_from_stek(stk)
