#include "esrin.h"

struct tok_list *ast_tree;

struct AST_list{
	struct tok_list *first;
	struct tok_list *last;
} *ast_list;

 

/* Error handling code goes here */
 

/*phase 1 error passing  */
int err_phs_1[] = {
 CAPET ,  
 SON ,
 DAXIL ,
 EGER ,
 YOXSA ,
 VE ,
 VEYA ,
 FUNKSIYA ,
 QAYTAR ,
 NEQEDERKI ,
 DAVAMET ,
 DAYAN ,
 SEC ,
 HAL ,
 SUSMA ,
 STRUKT ,
 SINIF ,
 MEXSUSI ,
 UMUMI ,
 QORUNAN ,
 STANDART ,
 SAY,
 ASGN,
 NOQTEV
};

/*phase 2 error passing  */
int err_phs_2[] = {
KODDATA,
EXPR
};

struct context_left_typ {
	int id;
	int context;
};

struct context_left_typ context_left[] = {
	{DAXIL,  DAXIL_INSIDE},
	{CAPET,  CAPET_INSIDE},
	{EGER,   EGER_INSIDE},  
	{EGER_EXP1,   EGER_INSIDE},
	{EGER_EXP2,   EGER_INSIDE},
	{ARYOPN, ARRAY_INDEX},
	{FCALL,  FCALL_VDEC}, 
	{GRT,  SHERT_RIGHT}, 
	{LES,  SHERT_RIGHT}, 
	{BERABER,  SHERT_RIGHT}, 
	{FERQLI,  SHERT_RIGHT}, 
    {GRTEQU,  SHERT_RIGHT}, 
	{LESEQU,  SHERT_RIGHT}, 
	{DAXILET1,  DAXIL_INSIDE}, 
	{CAPET1,  CAPET_INSIDE}, 
	{DAXILET2,  DAXIL_INSIDE}, 
	{CAPET2,  CAPET_INSIDE}, 
	{DAXILET3,  DAXIL_INSIDE}, 
	{CAPET3,  CAPET_INSIDE}, 
	{ASGN,  ASGN_RIGHT},  
	{SAY, SAY_INSIDE },
	{FUNKSIYA,  FUNKSIYA_ELAN },
	{OPNBLK,  BLOCK_INSIDE },
	{SINIF, SINIF_INSIDE},  /* burda problemler var bilirem(sinifle bagli)  */
	{NOQTEV, FREE_CONTEXT}
};

/* yes manually , please don't forget to update !!!!! */
int context_left_count = 25;




/* convert {  } into block */
/* convert BLOCK  BLOCK into block */
int cons_ast_block2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,OPNBLK) && matchid(b,CLSBLK))|| (matchid(a,BLOCK) && matchid(b,BLOCK))){
       ptr->id  = BLOCK;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
	   ptr->lrb = 1; //branch

	   ptr->next = tll->next->next; 
	   tllp->next = ptr;   
       *tll_ref = ptr;
	   (*pos)++;
       return 1;
  }

out:
  return 0;
}


/* convert {  BLOCK } into block */
int cons_ast_block3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (matchid(a,OPNBLK) && matchid(b, BLOCK) && matchid(c,CLSBLK)){
       ptr->id  = BLOCK;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;
	   ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
	   ptr->lrb = 1; //branch

	   ptr->next = tll->next->next->next; 
	   tllp->next = ptr;   
       *tll_ref = ptr;
	   (*pos)++;
       return 1;
  }

out:
  return 0;
}


/* convert SINIF BLOCK into SINIF_TYPE */
int cons_ast_sinif2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF) && matchid(b,BLOCK)){
       ptr->id  = SINIF_TYPE;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;
	   
       ptr->cld_cnt   = 2;
	   ptr->lrb = 1; //branch

	   ptr->next = tll->next->next; 
	   tllp->next = ptr;   
       *tll_ref = ptr;
	   (*pos)++;
       return 1;
  }

out:
  return 0;
}


/* convert SINIF {  FUNK } into SINIF_TYPE */
int cons_ast_sinif4_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (tll->next->next->next!=NULL)
	  d = tll->next->next->next->id;
  else
    return 0;

  if (matchid(a,SINIF) && matchid(b,OPNBLK) && matchid(c, FUNK) && matchid(d,CLSBLK)){
       ptr->id  = SINIF_TYPE;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;
	   ptr->childs[2] = tll->next->next;
	   ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
	   ptr->lrb = 1; //branch

	   ptr->next = tll->next->next->next->next; 
	   tllp->next = ptr;   
       *tll_ref = ptr;
	   (*pos)++;
       return 1;
  }

out:
  return 0;
}


/* convert FUNKSIYA BLOCK into FUNK */
/* convert FUNK FUNK into FUNK */
int cons_ast_funk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,FUNKSIYA) && matchid(b,BLOCK)) || (matchid(a,FUNK) && matchid(b,FUNK)) ){
       ptr->id  = FUNK;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
	   ptr->lrb = 1; //branch

	   ptr->next = tll->next->next; 
	   tllp->next = ptr;   
       *tll_ref = ptr;
	   (*pos)++;
       return 1;
  }

out:
  return 0;
}


int build_cur_layer_ast_block (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

	if (cons_ast_funk_tok(ptr, &tll,   &pos))
      continue;

	if (cons_ast_sinif2_tok(ptr, &tll,   &pos))
      continue;

	if (cons_ast_block2_tok(ptr, &tll,   &pos))
      continue;

	if (cons_ast_block3_tok(ptr, &tll,   &pos))
      continue;

  tll = tll->next;

   }

  return pos;
}

int build_cur_layer_ast_sinif (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

	if (cons_ast_sinif4_tok(ptr, &tll,   &pos))
      continue;

	tll = tll->next;

   }

  return pos;
}



void bottom_up_parse_ast_block(struct tok_list *tree){

    while(build_cur_layer_ast_block(tree));
 	while(build_cur_layer_ast_sinif(tree));   
}


int get_context_left(int *current_context, int id){

	int i;

	for (i=0; i<context_left_count; ++i){
		if (context_left[i].id == id){
			*current_context = context_left[i].context;
			return context_left[i].context;
		}
	}

	return *current_context;
}

void init_context_left(){

	struct tok_list_el *tll = tree->first;
	int current_context = FREE_CONTEXT;

	while (tll != tree->last){
		tll->context_left = get_context_left(&current_context, tll->id);
		tll = tll->next;	
	}
}

/* init trees tok_list_els (laytest layer) prev links 
   and trees last nodes reference  */
void init_prev_links(){

	struct tok_list_el *prev, *tll = tree->first->next;

	tll->prev = tree->first; /* Should be FILEBEG  */

	while(tll->id != FILESON){
		prev = tll;
		tll = tll->next;
		tll->prev = prev;	
	}

	tree->last = tll->next; /* one more FILESON reserve  */
}



int is_kwd(int id){

	int i, count;

	count  = sizeof(err_phs_1);

	for (i = 0; i< count; ++i)
		if (err_phs_1[i] == id)
			return 1;

	return 0;
}

void print_only_sinif_and_funcs(){


  ast_tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  ast_tree->first = ast_tree->last = NULL;
  ast_tree->count = 0;

  init_bottom_layer_new(ast_tree);

	struct tok_list_el *tll = ast_tree->first->next;

	printf("printing by sinif and funcs\n");

	while (tll->id	!= FILESON){

		if (matchid(tll->id, SINIF) || matchid(tll->id, FUNKSIYA)){
			printf("\n  -->  \t");
			print_tok(tll->id);
			printf("\n  ");
		}
		else{
			print_tok(tll->id);
			printf(" ");
		}
	
		tll = tll->next;
	}
}


void add_ast_list(struct AST_list *a_list, int id){

	struct tok_list *tk_list;

	//create tok_list
	tk_list = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
	tk_list->first = tk_list->last = NULL;
	tk_list->next = NULL;
	tk_list->id = id;

	//add tok_list to a list
	if (a_list->last == NULL){
		a_list->first = a_list->last = tk_list;
	}
	else{
		a_list->last->next = tk_list;
		a_list->last = tk_list;
	}

}

void add_ast_elem(struct tok_list *tk_list, struct tok_list_el *tll){

	struct tok_list_el *atll;

	atll = (struct tok_list_el *)malloc(sizeof(struct tok_list_el));
	//copy tll to all
	*atll = *tll;
	//remove references
	atll->next = atll->prev = NULL;
	//now atll ready to be added to list

	if (tk_list->last == NULL){
		tk_list->first = tk_list->last = atll;
	}
	else{
		tk_list->last->next = atll;
		tk_list->last = atll;
	}

}

void init_ast_list(){

  struct tok_list_el *tll;

  struct tok_list *tk_list;
  
  ast_tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  ast_tree->first = ast_tree->last = NULL;
  ast_tree->count = 0;

  init_bottom_layer_block(ast_tree);

  print_tree_dx(ast_tree);

  tll = ast_tree->first->next;

  ast_list = (struct AST_list *)malloc(sizeof(struct AST_list)) ;
  ast_list->first = ast_list->last = NULL;
  
  while (tll->id != FILESON){

   if (matchid(tll->id, SINIF) || matchid(tll->id, FUNKSIYA)){
		//add new list
	   add_ast_list(ast_list, tll->id);
	}
	else{
		//always add to the last list
		add_ast_elem(ast_list->last, tll);
	}

	tll = tll->next;
 }

}


void print_ast_list(struct AST_list *ast_list){

	struct tok_list *tk_list;

	printf("\n printing AST list: \n");

	tk_list = ast_list->first;

	while(tk_list != NULL){
		printf("\n  -->  \t");
		print_tok(tk_list->id);
		printf("\n    \t\t");
		print_tok_list(tk_list);
		tk_list = tk_list->next;
	}
}

void print_by_kwd(){

	struct tok_list_el *tll = tree->first->next;

	printf("printing erronous tokens\n");

	while (tll->id	!= FILESON){

		if (is_kwd(tll->id)){
			printf("\n Error here: ");
			print_tok(tll->id);
			printf("\n  ");
		}
		else{
			print_tok(tll->id);
			printf(" ");
		}
	
		tll = tll->next;
	}
}

void print_by_context(){

	struct tok_list_el *tll = tree->first->next;

	printf("printing tokens by context\n\n  ");

	while (tll->id	!= FILESON){
   		print_tok(tll->id);
		printf("   \tcontext_left:\t\t");
		print_tok(tll->context_left);
		printf("\n  ");
		tll = tll->next;
	}
}

void build_block_brackets_tok_list(struct tok_list *tk_list){

	struct tok_list_el *tll;
	int opn = 0;

	//init block stek to NULL
	tk_list->opn_blk_stk.top = NULL;
	tk_list->cls_blk_stk.top = NULL;

	tll = tk_list->first;

	while (tll != NULL){
	
		if (tll->id == OPNBLK)
			push_to_stek(&tk_list->opn_blk_stk, tll);
		else
			if (tll->id == CLSBLK){
				if (tk_list->opn_blk_stk.top != NULL)
					pop_from_stek(&tk_list->opn_blk_stk);
				else
					push_to_stek(&tk_list->cls_blk_stk, tll);
			}

		tll = tll->next;
	}

}


void build_block_brackets_ast_list(struct AST_list *a_list){

	struct tok_list *tk_list;

	tk_list = a_list->first;

	while (tk_list != NULL){

		build_block_brackets_tok_list(tk_list);
		tk_list = tk_list->next;	

	}

}


void check_block_brackets_tok_list(struct tok_list *tk_list){

	struct tok_list_el *tll;
	int opn = 0;

	//first check opn blocks
	while (tk_list->opn_blk_stk.top != NULL){
		tll = (struct tok_list_el *)pop_from_stek(&tk_list->opn_blk_stk);
		printf("sehv: setir %d -de elan olunan '{' blok moterizesi ucun baglayan '}' moterize tapilmadi. \n", tll->tok->row);
	}


	//close blocks
	while (tk_list->cls_blk_stk.top != NULL){
		tll = (struct tok_list_el *)pop_from_stek(&tk_list->cls_blk_stk);
		printf("sehv: setir %d -de elan olunan '}' blok moterizesi ucun acan '{' moterize tapilmadi. \n", tll->tok->row);
	}

}


void check_block_brackets_ast_tree(struct tok_list *ast_tree){

	struct tok_list_el  *tll;

	tll = ast_tree->first;

	while (tll != NULL){
		if (tll->id == OPNBLK)
			printf("sehv: setir %d -de elan olunan '{' blok moterizesi ucun baglayan '}' moterize tapilmadi. \n", tll->tok->row);
		else
			if (tll->id == CLSBLK)
				printf("sehv: setir %d -de elan olunan '}' blok moterizesi ucun acan '{' moterize tapilmadi. \n", tll->tok->row);

		tll = tll->next;
	}

}


void do_ast_job(){

	ast_tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
	ast_tree->first = ast_tree->last = NULL;
	ast_tree->count = 0;
  
	init_bottom_layer_block(ast_tree);
	bottom_up_parse_ast_block(ast_tree);

	check_block_brackets_ast_tree(ast_tree);
}


#if 0

//dont compile


	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");

	print_by_kwd();
	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");

	init_context_left();
	print_by_context();


	print_only_sinif_and_funcs();
	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");

	init_ast_list();
	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");
	print_ast_list(ast_list);

	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");
	printf("Building block brackets\n");
	build_block_brackets_ast_list(ast_list);

	printf("Checking block brackets\n");
	check_block_brackets_ast_list(ast_list);





#endif