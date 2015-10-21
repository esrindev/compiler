/* Lisence GPL V3   
   Ahmed Sadikhov 2014.22.oct
   ahmed.sadikhov@gmail.com
*/
#if 1
 
  #include "esrin.h"

extern void push_to_stek(struct stek *stk, void *dptr);
extern void *pop_from_stek(struct stek *stk);

extern int prg_start; /* pos of program on exec_band*/

void addto_band_val(struct tok_list_el *tll, struct band_list_el band[], int *size);
void addto_band_adrs(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last);
void build_exec_band(struct tok_list_el *tll, struct band_list_el band[], int *size);

struct band_list_el  exec_band[BAND_SIZE] ;
int band_size, prg_start;

/* if IDT, NUMB, FLOAt, CHR ten just tok, if CRG1/2 then childs[0]->tok, if STRMEM  then ... check code */
struct token *head_tok(struct tok_list_el *tll){

  struct tok_list_el  *head_tll;
  struct token *head_tok;

  if (tll->id == STRMEM || tll->id == SNMEM)
    head_tll = tll->childs[0];
  else
    head_tll = tll;

  switch(head_tll->id){
  case IDT:
  case NUMB:
  case FLOAT:
  case CHR:
  case EXPR:/* why EXPR is here ???????? */
    head_tok = head_tll->tok;
    break;
  case CRG1:
  case CRG2:
    head_tok = head_tll->childs[0]->tok;
    break;
  default:
    head_tok = head_tll->tok;
  }

  return head_tok;
}


struct tok_list_el *create_tok_list_el(struct token *tok){

  struct tok_list_el *tmp = (struct tok_list_el *)malloc(sizeof(struct tok_list_el));

  tmp->tok  = tok;
  tmp->id   = tok->id;
  /*No Child*/
  tmp->childs[0] = NULL;
  tmp->cld_cnt   = 0;
  tmp->next = NULL;
  tmp->old_copy = 0;
  tmp->lrb = 0;
  
  return tmp;

}


void tok_list_insert_el(struct tok_list *tl, struct tok_list_el **tll_ref, struct tok_list_el *tll_end, struct tok_list_el *ptr){

  (*tll_ref)->next = ptr;
  ptr->next = tll_end;
  tl->count++;      

}


void tok_list_add_el(struct tok_list *tl, struct tok_list_el *tmp){

  if (tl->count == 0){
    tl->first = tl->last = tmp;
      }
  else{
    tl->last->next = tmp;
    tl->last = tmp;
  }

  tl->count++;      

}


void init_bottom_layer_new(struct tok_list *tree){
  
int i;

  struct tok_list_el *tll;

  for (i=0; i<tks; ++i){
    tll = create_tok_list_el(&tokens[i]);
    tok_list_add_el(tree,tll);
  }

}


void addto_list(struct tok_list_el *tll, struct tok_list *plst){

  struct tok_list_el *tmp;

  //  tmp = create_new_tll(tll);
  tmp = (struct tok_list_el *)malloc(sizeof(struct tok_list_el));
  *tmp = *tll;

  if (plst->count == 0){
    /* first element  */
    plst->first = plst->last = tmp;
    plst->count = 1;
    tmp->next = tmp->prev = NULL;
  }
  else{
    plst->last->next = tmp;
    tmp->prev = plst->last;
    plst->last = tmp;
    tmp->next = NULL;
    (plst->count)++;
  }
}


void free_dovr_stek(struct stek *stk, int pos, int pos2){

  struct band_list_el *bll;
  void *ptr;

  while (stk->top != NULL){

	ptr = pop_from_stek(stk);
    bll = (struct band_list_el *)ptr;

    switch(bll->band_id){
    case INIT_DOVR:
    case NEQEDERKI2:
      return;
    case DAVAMET1:
      bll->head_pos = pos2;
      break;
    case DAYAN1:
      bll->head_pos = pos;
      break;
      // default:
      //error
    }

  }
}

void free_secim_stek(struct stek *stk, int pos ){

  struct band_list_el *bll;

  while (stk->top != NULL){

    bll = (struct band_list_el *)pop_from_stek(stk);

    switch(bll->band_id){
    case FREE_SEC_DYN_STK:
      return;
    case DAVAMET1:
      printf("Sehv: davamet operatoru dovr daxilinde olmalidir.\n");
      break;
    case DAYAN1:
      bll->head_pos = pos;
      break;
    }
  }
}

void addto_band_new(struct tok_list_el *tll, struct band_list_el band[], int *size, int id, int hpos){

  struct tok_list_el *tmp;
  struct token *tok;

  if (*size > BAND_SIZE - 1){
	  printf("Sehv: Icra lenti dolub.\n");
	err_cnt++;
    return;
  }

  tok = head_tok(tll);

  band[*size].id = tll->id;
  band[*size].band_id = id;
  strcpy(band[*size].tok, tok->tok);
  band[*size].d1 = tok->d1;
  band[*size].d2 = tok->d2;
  band[*size].row = tok->row;
  band[*size].col = tok->col;
  band[*size].tip = tok->tip;
  band[*size].size = tok->size;
  band[*size].lgm = tok->lgm;
  band[*size].ofst = tok->ofst;
  band[*size].fid = tok->fid;
  band[*size].sntip = tok->sntip;
  band[*size].head_pos = hpos;
  (*size)++;

}

int sntip = GLB_TIP;
int fid = 0;


struct tok_list_el *addto_band_ofst(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last){

  struct tok_list_el *head, *ofst;

    
  switch(tll->id){
  case IDT:
  case CRG1:
  case CRG2:
  case FCALL1:
    head = tll;
    ofst = NULL;
    break;
  case STRMEM:
  case SNMEM:    
    head = tll->childs[0];
    ofst = tll->childs[2];
    break;
  }

  switch(head->id){
  case IDT:
    addto_band_new(head, band, size, IDT_OFST, 0);
    break;
  case CRG1:
    addto_band_val(head->childs[2], band, size);
    addto_band_new(head->childs[0], band, size, CRG1_OFST, 0);
    break;
  case CRG2:
    addto_band_val(head->childs[2], band, size);
    addto_band_val(head->childs[5], band, size);
    addto_band_new(head->childs[0], band, size, CRG2_OFST, 0);
    break;
  }
  
  if (ofst == NULL)
    *tll_last =  head;
  else
    addto_band_ofst(ofst, band, size, tll_last);

  return NULL; /* error  */
}

void addto_band_val(struct tok_list_el *tll, struct band_list_el band[], int *size){

  struct tok_list_el *tll_last;

  switch(tll->id){
  case ARTSIN:
    addto_band_new(tll, band, size, ARTSIN, 0);
    break;
  case AZALSIN:
    addto_band_new(tll, band, size, AZALSIN, 0);
    break;
  case NUMB:
    addto_band_new(tll, band, size, NUMB, 0);
    break;
  case FLOAT:
    addto_band_new(tll, band, size, FLOAT, 0);
    break;
  case CHR:
    addto_band_new(tll, band, size, CHR, 0);
    break;
  case IDT:
    addto_band_new(tll, band, size, IDT_VAL, 0);
    break;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0], band, size, CRG1_VAL, 0);
    break;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0], band, size, CRG2_VAL, 0);
    break;
  case EXPR:
    addto_band_val(tll->childs[0], band, size);
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[1], band, size, EXPR, 0);
    break;
  case SHERT:
    addto_band_val(tll->childs[0], band, size);
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[1], band, size, SHERT, 0);
    break;  
  case STRMEM:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_ofst(tll->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, STRMEM_VAL, 0);
    break;
  case SNMEM:
  case FCALL1:
    build_exec_band(tll, band, size);
    break;
  }

}

void addto_band_adrs(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last){

  int i,id;

  switch(tll->id){
  case IDT:
    addto_band_new(tll, band, size, IDT_ADRS, 0);
    *tll_last = tll;
    break;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0], band, size, CRG1_ADRS, 0);
    *tll_last = tll->childs[0];    
    break;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0], band, size, CRG2_ADRS, 0);
    *tll_last = tll->childs[0];    
    break;
 case STRMEM:
   addto_band_adrs(tll->childs[0], band, size, tll_last);
   addto_band_ofst(tll->childs[2], band, size, tll_last);
   break;
  case CHR:
    addto_band_new(tll, band, size, CHR_ADRS, 0);
    *tll_last = tll;
    break;
  case STR:
    addto_band_new(tll, band, size, STR_ADRS, 0);
    *tll_last = tll;
    break;
  }

}

void addto_band_capet(struct tok_list_el *tll, struct band_list_el band[], int *size){
  int i;
  struct tok_list_el *tll_last;

  switch(tll->id){
  case NUMB:
    addto_band_new(tll,band, size, NUMB, 0);
    addto_band_new(tll,band, size, CAPET_NUMB,0);    
    return;
  case FLOAT:
    addto_band_new(tll,band, size, FLOAT, 0);
    addto_band_new(tll,band, size, CAPET_FLOAT, 0);
    return;
  case CHR:
    addto_band_new(tll,band, size, CHR, 0);
    addto_band_new(tll_last, band, size, CAPET_CHR, 0);
    return; 
  case STR:
    addto_band_adrs(tll,band, size, &tll_last);
    addto_band_new(tll,band, size, CAPET_STR, 0);
    return;
  case IDT:
    addto_band_new(tll,band, size, IDT_VAL, 0);
    addto_band_new(tll,band, size, CAPET_IDT, 0);
    return;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0],band, size, CAPET_CRG1, 0);
    return;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0],band, size, CAPET_CRG2, 0);
    return;
  case STRMEM:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_ofst(tll->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, CAPET_STRMEM, 0);
    return;
  case EXPR:
    addto_band_val(tll, band, size);
    addto_band_new(tll, band, size, CAPET_EXPR, 0);
    return;
  }
  
  for (i=0; i < tll->cld_cnt ; ++i)
    addto_band_capet(tll->childs[i], band, size);

}


void addto_band_daxilet(struct tok_list_el *tll, struct band_list_el band[], int *size){
  int i;
  struct tok_list_el *tll_last;

  switch(tll->id){
  case IDT:
  case CRG1:
  case CRG2:
  case STRMEM:
    addto_band_adrs(tll, band, size, &tll_last);
    addto_band_new(tll_last,band, size, DAXILET_IDT, 0);
    return;
  }
  
  for (i=0; i < tll->cld_cnt ; ++i)
    addto_band_daxilet(tll->childs[i], band, size);
}



void add_fargs_to_band(struct tok_list_el *tll, struct band_list_el band[], int *size){

  int i ;

  switch(tll->id){
  case NUMB:
  case FLOAT:
  case CHR:
  case STR:
  case IDT:
  case CRG1:
  case CRG2:
  case STRMEM:
  case EXPR:
  addto_band_val(tll, band, size);
  return ;
  }

    for (i = 0; i<tll->cld_cnt; ++i)
      add_fargs_to_band(tll->childs[i], band, size);

    return;
}


void add_fparms_to_band(struct tok_list_el *tll, struct band_list_el band[], int *size){

  struct tok_list_el *tmp;
  int i, old_size, id = tll->id;
  struct tok_list_el *tll_last;

    /* currently  we support only passing variables of type 1,2 or 3 and not arrays to functions */
    if (tll->cld_cnt == 2 || tll->id == IDT ){
    addto_band_new(tll->childs[1], band, size, FPARM_IDT_VAL, 0);
    return ;
  }
  

  /* add args in revers order for stack reasons  */
    for (i = tll->cld_cnt - 1; i>=0; --i)
      add_fparms_to_band(tll->childs[i], band, size);

    return;
}

void addto_band_HAL1S(struct tok_list_el *tll, struct band_list_el band[], int *size, int *hal1_cnt){

  int i;

  if (tll->id == HAL1){
    addto_band_val(tll->childs[1], band, size);
    addto_band_new(tll->childs[0], band, size, HAL1S, 0); 
    *hal1_cnt += 2;
    return;
  }

  for (i=0; i<tll->cld_cnt; ++i)
    addto_band_HAL1S(tll->childs[i], band, size, hal1_cnt);

  return;
}


void build_exec_band(struct tok_list_el *tll, struct band_list_el band[], int *size){

  int i, hpos, old_size, old_size1, old_size2, id = tll->id;
  struct tok_list_el *tmp, *tll_last;
  struct token *tok;
  
  if (*size > BAND_SIZE){
    printf("Icra lenti dolub.\n");
    return;
  }

  switch(id){

  case START:
  case YENI_TIP:
  case SINIF_DECL:
  case SINIF_BODY:
  case PRG2:
  case FUNKS:
  case FUNK_BODY:
  case KOD1:
  case KODDATA:
  case HAL2:

    if (tll->childs[0]->id == DOVR_EXP){
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[4], band, size);
    addto_band_new(tll_last, band, size, INIT_DOVR, 0); /* INIT_DOVR  */
      push_to_stek(&dovrstk, &band[*size-1]);
      
      old_size = *size; /* we'll jump here from STEP_DOVR  */
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[7], band, size);
      addto_band_val(tll->childs[0]->childs[3], band, size);
      old_size1 = *size;
    addto_band_new(tll_last, band, size, CHECK_DOVR, 0); /* CHECK_DOVR  */

         build_exec_band(tll->childs[1], band, size);

      old_size2 = *size;
      addto_band_val(tll->childs[0]->childs[3], band, size);
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, STEP_DOVR, old_size); /* STEP_DOVR  */
      /* jump out of loop to next instr. 3 is ofst from INIT_DOVR  */     
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size2);
      break;
    }


    if (tll->childs[0]->id == NEQEDERKI2){
      old_size = *size;
      addto_band_val(tll->childs[0]->childs[1], band, size);      
     // push_to_stek(&dovrstk, tll->childs[0]);
      old_size1 = *size;
      addto_band_new(tll->childs[0], band, size, NEQEDERKI2, 0); 
	  push_to_stek(&dovrstk, &band[*size-1]);
      build_exec_band(tll->childs[1], band, size);      
      addto_band_new(tll->childs[0], band, size, NEQ_TOP, old_size);      
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size);
      break;
    }

	    //addto_band_new(tll, band, size, tll->id, 0);
	 //push_to_stek(&dovrstk, &band[*size-1]);


    if (tll->childs[0]->id == EGER_EXP2){
      if (tll->cld_cnt == 2){ // if ...
	addto_band_val(tll->childs[0]->childs[1], band, size);
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, EGER_EXP2, 0);
	build_exec_band(tll->childs[1], band, size);
	band[old_size].head_pos = *size;  	
      }
      else{ // if ... else ...
	addto_band_val(tll->childs[0]->childs[1], band, size);
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, EGER_EXP2, 0);	
	build_exec_band(tll->childs[1], band, size);
	band[old_size].head_pos = *size + 1;  	 // +1 for else
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, YOXSA, 0);		
	build_exec_band(tll->childs[3], band, size);	
	band[old_size].head_pos = *size;  		
      }
      break;      
    }


    if (tll->childs[0]->id == SEC1){
      old_size = *size;
      addto_band_adrs(tll->childs[1], band, size, &tll_last);
	  addto_band_new(tll->childs[0], band, size, FREE_SEC_DYN_STK, 0); 
	  push_to_stek(&dovrstk, &band[*size-1]);
      build_exec_band(tll->childs[4], band, size);      
      addto_band_new(tll->childs[0], band, size, SEC1, 0); 
      free_secim_stek(&dovrstk, *size);
      /* just rem switch variable from address stek  */	
      break;      
    }


    /*

      secim(x){
      hal 1:
        capet(1);
	dayan;
      hal 2:
        capet(2);
	dayan;
      hal 3:
      hal 4:
      hal 5:
      hal 6:
       capet(3,4,5,6);
       dayan;	
      sus:
       capet(0);
      }

   if x == 1 then shift head_pos to next pos to  exec hal 1 code, else jump to hal2
  but not same for multiple cases. 
  if x == 3 DO NOT SHIFT to next pos but JUMP to head_pos to exec kod, else shift to check next cases

   as we see 1 case and multiple cases work vise versa. 

     */

    if (tll->id == HAL2){

      if (tll->childs[0]->id == HAL1){
	addto_band_val(tll->childs[0]->childs[1], band, size);
	addto_band_new(tll->childs[0], band, size, HAL1, 0);
	old_size = *size;
	build_exec_band(tll->childs[1], band, size);      
	band[old_size-1].head_pos = *size;  		      
	break;     
      }

      if (tll->childs[0]->id == SUS1){
	addto_band_new(tll->childs[0], band, size, SUS1, 0); 
	build_exec_band(tll->childs[1], band, size);      
	break;    
      }

      if (tll->childs[0]->id == HAL1S){
	int j, hal1_cnt = 0;
	old_size = *size;
	addto_band_HAL1S(tll->childs[0], band, size, &hal1_cnt);
	old_size1 = *size;
	build_exec_band(tll->childs[1], band, size); 
	for (j=1; j<hal1_cnt; j += 2)
	  band[old_size + j].head_pos = old_size1;  		      
	band[old_size1 - 1].head_pos = *size;  
	band[old_size1 - 1].band_id = HAL1;
	break;      
      }

      build_exec_band(tll->childs[0], band, size);      
      build_exec_band(tll->childs[1], band, size);      
      break;
    }


    for (i = 0; i<tll->cld_cnt; ++i)
      build_exec_band(tll->childs[i], band, size);
    break;

  case DAVAMET1:
  case DAYAN1:    
    addto_band_new(tll, band, size, tll->id, 0);
    push_to_stek(&dovrstk, &band[*size-1]);
    break;
  case CAPET3:    
    addto_band_capet(tll->childs[0]->childs[1], band, size);
    break;
  case DAXILET3:
    addto_band_daxilet(tll->childs[0]->childs[1], band, size);
    break;
  case ASGN_OP:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll_last, band, size, ASGN, 0); 
    break;


  case SINIF_TYPE:
    sntip++ ;
    fid = 0;
    build_exec_band(tll->childs[1], band, size);
    /* set fid to 1 when leaving sinif type to prevent 
       GLB funcs to override MAIN_FUNC   */
    break;
  case FUNK:
    tt->tips[sntip].ft.funcs[fid].head_pos = *size;
    //printf("function %s asgnd: sntip %d, fid %d,  hpos %d.\n", tll->childs[0]->childs[0]->childs[1]->tok->tok, sntip, fid, *size);
    tok = tll->childs[0]->childs[0]->childs[1]->tok;
    tok->sntip = sntip;
    tok->fid = fid;
    addto_band_new(tll->childs[0]->childs[0]->childs[1], band, size, FUNK, 0); 
    if (tll->childs[0]->cld_cnt == 3) /* add fparms to band */
      add_fparms_to_band(tll->childs[0]->childs[1], band, size);
    build_exec_band(tll->childs[1], band, size);
    addto_band_new(tll->childs[0], band, size, QAYTAR1, 0); 
    fid++;
    break;

    /* in above case (FUNK) we add QAYTAR1 to band at the end. This is just
       to return properly if user will not write qaytar for void funcs.
       next case (QAYTAR1) however is used for returning value or by user
       request. */

  case QAYTAR1:
    if (tll->cld_cnt == 3){
      addto_band_val(tll->childs[1], band, size);      
      addto_band_new(tll, band, size, QAYTAR1, 0); 
    }
      
    break;

  case FCALL1:
    /* push return address to glbstk  */
    /* TODO:  remove tmp which is not clear why is it here */
    old_size = *size;
    addto_band_new(tll, band, size, PUT_RET_ADDR, 0); 
    if (tll->cld_cnt == 3) /* add fargs to band */
      add_fargs_to_band(tll->childs[1], band, size);
    tok = tll->childs[0]->childs[0]->tok;
    hpos = tt->tips[tok->sntip].ft.funcs[tok->fid].head_pos;
    addto_band_new(tll->childs[0]->childs[0], band, size, FCALL1, hpos);
    /* set dt_area and fields for funcs  */
    band[*size - 1].dt_size = tt->tips[tok->sntip].ft.funcs[tok->fid].dt.size;
    band[old_size].head_pos = *size;
    break;

  case SNMEM:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_ofst(tll->childs[2], band, size, &tll_last);
    old_size = *size;
    addto_band_new(tll, band, size, PUT_RET_ADDR, 0); 
    if (tll_last->cld_cnt == 3) /* add fargs to band */
      add_fargs_to_band(tll_last->childs[1], band, size);
    tok = tll_last->childs[0]->childs[0]->tok;
    hpos = tt->tips[tok->sntip].ft.funcs[tok->fid].head_pos;
    addto_band_new(tll_last->childs[0]->childs[0], band, size, SNMEM, hpos);
    band[*size - 1].dt_size = tt->tips[tok->sntip].ft.funcs[tok->fid].dt.size;
    band[old_size].head_pos = *size;
    addto_band_new(tll_last, band, size, POP_OBSTK, 0);
    break;

  case PRG1:
    prg_start = *size;
    /* above code is for linux  */
    band[0].head_pos = prg_start;
    sntip = GLB_TIP;
    build_exec_band(tll->childs[tll->cld_cnt - 2], band, size); //see parsing PRG1
    addto_band_new(tll->childs[0], band, size, SON, 0); 
    fid = 1;


    
    break;
     /*
  case FCALL1:
    return exec_func(tll, ptll, NULL);
  case SNMEM:
    return exec_snmem(tll, ptll);
  case QAYTAR1:
    return exec_qaytar1(tll, ptll);
  case DAVAMET1:
    return exec_davamet1(tll, ptll);
  case DAYAN1:
    return exec_dayan1(tll, ptll);
     */

  }
}

void print_exec_band(struct band_list_el band[], int size){

  int i;

  for (i=0; i<size; ++i){
    printf("hpos -> %d\t",i);
    print_tok(band[i].id);
    printf("\t");
    print_tok(band[i].band_id);
    printf("\thpos = %d lgm %d tok %s", band[i].head_pos, band[i].lgm, band[i].tok);
    if (band[i].band_id == FUNK || band[i].band_id == FCALL1 || \
	band[i].band_id == SNMEM)
      printf("%s sntip %d  fid  %d  dt_size %d\n ", band[i].tok, band[i].sntip, band[i].fid, band[i].dt_size);
    else{
      if (band[i].band_id == GLBMT)
	printf(" dt_size  %d", band[i].dt_size);
      printf("\n");
    }
  }
}


void init_funcs_ret_addrs(struct band_list_el band[], int size)
{

  int i,j;

  for (i=0; i<size; ++i){
    if (band[i].band_id == FUNK){
      for (j=0; j<size; ++j){
	if (band[j].band_id == FCALL1 || band[j].band_id == SNMEM){
	  if (band[i].sntip == band[j].sntip && band[i].fid == band[j].fid)
	    band[j].head_pos = i;
	}
      }
    }
  }
}

struct pre_stek_el{
  long double el;
  struct pre_stek_el *next;
};

struct pre_stek{
  struct pre_stek_el *top;
  int count;
};


void push_to_pre_stek(struct pre_stek *st, long double k){

  struct pre_stek_el *stl;

  stl = (struct pre_stek_el *)malloc(sizeof(struct pre_stek_el));
  stl->el = k;
  stl->next = st->top;
  st->top = stl;
  (st->count)++;
}

struct pre_stek_el *pop_from_pre_stek(struct pre_stek *st){

  struct pre_stek_el *stl;

  if (st->count != 0){
    stl = st->top;
    st->top = st->top->next;
    stl->next= NULL;
    (st->count)--;
    return stl;
  }

  return NULL; /* steck is empty  */
}





int print_layer_dx_tr(struct tok_list_el *tll, int depth, int n){

  int i,m;

  m = 0;

  if (depth == n){
    print_tok(tll->id);
    printf(" ");
    //    printf(" %s %p ",tll->tok->tok, tll->tok);

    if (tll->lrb == 1)
      return 1;   
    else
      return 0;
  }

  if (tll->lrb == 0)
    return 0;

  for (i=0; i<tll->cld_cnt; ++i)
    m+=print_layer_dx_tr(tll->childs[i], depth, n+1);   

  return m;

}


int print_layer_dx(struct tok_list *tl, int depth){

  struct tok_list_el *tll;
  int  k;

  k = 0;

  tll = tl->first;
  
  while (tll!=NULL){
    k+=print_layer_dx_tr(tll, depth, 1);
    tll = tll->next;
  }
  
  return k;
}


void print_tree_dx(struct tok_list *tl){

  int depth, cnt;

  depth = 0;
  cnt = 1;

  while(cnt){
    depth++;
    cnt = print_layer_dx(tl, depth);
    printf("\n");
 }
}


#endif