#include "esrin.h"
#include "rules_data_base.h"

// get index of rule_set from rule_token_set
//for given id
//extern build_cur_layer_hp_oprs(tree);
//extern build_cur_layer_lp_oprs(tree);

int err_cnt = 0; /* global error counter */
int wrn_cnt = 0; /* global warning counter */

char *pt;
int row, col; /* used to format error message */

char err_msg[ERR_MSG_LEN];

struct tok_list *tree_copy;

extern int parser_core();
extern void print_tok_str(int tok);
extern void sprint_tok_str(int tok, char *msg);
extern void make_copy_of_toklist(struct tok_list *tree, struct tok_list *tree_copy);
extern int get_tll_count(struct tok_list *tree);

void insert_tll_next_index( int index, int id);
int is_terminal(int id);

struct sehv{
	int row;
	int col;
	char msg[ERR_MSG_LEN];
	char *msg_ptr;
} sehvler[MAX_ERR_CNT];


struct ins_tll{

	//struct tok_list_el *ins_pos;
	char msg[ERR_MSG_LEN];
	int ins_pos;
	int ins_id;
	int decr_cnt; //count of tlls that insertion of this tll removed
} ins_tll_ary[MAX_ERR_CNT];

int ins_tll_cnt = 0;

int get_ins_tll_index(struct tok_list *tree, struct tok_list_el *tll){

	struct tok_list_el *ptll = tree->first;
	int ind = 0;

	while(ptll != NULL){
	
		if (ptll == tll)
			return ind;

		ptll = ptll->next;
		ind++;
	}

	printf("compilyator sehvi: get_ins_tll_pos -1 qaytardi\n");
	return -1;
}

void add_ins_tll(struct tok_list *tree, struct tok_list_el *tll, int id){

	int index;

	index = get_ins_tll_index(tree, tll);

	ins_tll_ary[ins_tll_cnt].ins_pos = index;
	ins_tll_ary[ins_tll_cnt].ins_id = id;
	ins_tll_cnt++;
}

void print_ins_tll_ary(){

	int i;

	for (i=0; i<ins_tll_cnt; ++i){
		printf("must insert ");
		print_tok(ins_tll_ary[i].ins_id);
		printf(" after ");
		
		//print_tok(ins_tll_ary[i].);


		printf("decr cnt is %d", ins_tll_ary[i].decr_cnt );

		printf("\n");
	}


}

void add_to_errors(int row, int col, char *msg){

	int size = 0;

	sehvler[err_cnt].row = row;
	sehvler[err_cnt].col = col;
	sehvler[err_cnt].msg_ptr = sehvler[err_cnt].msg; // will be changed during error sortgin
	strcpy(sehvler[err_cnt].msg_ptr , msg);
	err_cnt++;
}


void get_row_col_right(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far right leave, get row and col  */
    *row =  tll->tok->row;
    *col =  tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far right branch  */
  get_row_col_right(tll->childs[tll->cld_cnt-1], row, col, pt);
}

void get_row_col_left(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far left leave, get row and col  */
    *row = tll->tok->row;
    *col = tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far left branch  */
  if (tll->childs[0] != NULL)
	get_row_col_left(tll->childs[0], row, col, pt);
}

/* print expected string without surrounding ``  */
void print_before(struct tok_list_el *tll, char *msg){

    get_row_col_left(tll, &row, &col, &pt);    
	memset(err_msg, '\0', ERR_MSG_LEN);
    sprintf(err_msg, "(%d:%d)sehv: `%s` -dan evvel %s gozlenirdi.\n", row, col, pt,msg);    
	add_to_errors(row, col, err_msg);
	memset(ins_tll_ary[ins_tll_cnt].msg, '\0', 1024);
	sprintf(ins_tll_ary[ins_tll_cnt].msg, "%s", err_msg);
}

void print_after(struct tok_list_el *tll, char *msg){

	get_row_col_right(tll, &row, &col, &pt); 
	memset(err_msg, '\0', ERR_MSG_LEN);
    sprintf(err_msg, "(%d:%d)sehv: `%s` -dan sonra %s gozlenirdi.\n", row, col, pt,msg);    
	add_to_errors(row, col, err_msg);
	memset(ins_tll_ary[ins_tll_cnt].msg, '\0', 1024);
	sprintf(ins_tll_ary[ins_tll_cnt].msg, "%s", err_msg);
}


void print_type_not_def_err(struct token *tok){

		memset(err_msg, '\0', ERR_MSG_LEN);
		sprintf(err_msg, "(%d:%d)sehv: %s tipi elan olunmayib\n", tok->row, tok->col, tok->tok);
		add_to_errors(row, col, err_msg);
}

void print_var_dbl_dec_err(struct token *tokk, struct token *tokj, char *name){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s sinfinin %s adli deyishen heddi artiq elan olunub(setir %d).\n", tokk->row, tokk->col, name, tokk->tok,tokj->row);	
	add_to_errors(tokk->row, tokk->col, err_msg);
}

void print_member_not_found_err(struct token *tok){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s heddi tapilmadi\n", tok->row, tok->col, tok->tok);
	add_to_errors(tok->row, tok->col, err_msg);
}
	  
void print_var_not_decld_err(struct token *tok){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s deyisheni elan olunmayib \n", tok->row, tok->col, tok->tok);
	add_to_errors(tok->row, tok->col, err_msg);
}


void print_func_not_found_err(struct token *tok){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv:  %s funksiyasi tapilmadi\n", tok->row, tok->col,  tok->tok);
	add_to_errors(tok->row, tok->col, err_msg);
}


void print_std_type_decl_err(struct token *tok, char *name ){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s adi ile yeni tip elan etmek olmaz(standart tipdir).\n", tok->row, tok->col, name);
	add_to_errors(tok->row, tok->col, err_msg);

}


void print_type_dbl_dec_err(struct token *tokj, struct token *toki, char *name){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s adli tip artiq elan olunub(setir %d).\n", tokj->row, tokj->col, name,toki->row);
	add_to_errors(tokj->row, tokj->col, err_msg);
}

void print_dbl_dec_var_loc_err(struct token *tokm, struct token *tokk, char *name){

		memset(err_msg, '\0', ERR_MSG_LEN);
		sprintf(err_msg, "(%d:%d)sehv: %s funksiyasinin %s adli deyisheni artiq elan olunub(setir %d).\n", tokm->row, tokm->col, name, tokm->tok,tokk->row);	
		add_to_errors(tokm->row, tokm->col, err_msg);
}

void print_dbl_dec_var_glb_err(struct token *tokk, struct token *tokj, char *name){

		memset(err_msg, '\0', ERR_MSG_LEN);
		sprintf(err_msg, "(%d:%d)sehv: %s adli deyishen artiq elan olunub(setir %d).\n", tokk->row, tokk->col,name,tokj->row);	
		add_to_errors(tokk->row, tokk->col, err_msg);
}
						
void print_dbl_dec_func_err(struct token *tokk, struct token *tokj, char *name){

		memset(err_msg, '\0', ERR_MSG_LEN);
		sprintf(err_msg, "(%d:%d)sehv: %s adli funksiya artiq elan olunub(setir %d).\n", tokk->row, tokk->col,tokj->tok, tokj->row);	
		add_to_errors(tokk->row, tokk->col, err_msg);
}
						

void print_func_more_parms_err(struct token *fdtok, struct token *fstok, char *name){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s funksiyasina teleb olunan saydan artiq arqument oturulur(setir %d).\n",fdtok->row, fdtok->col, name, fstok->row );
	add_to_errors(fdtok->row, fdtok->col, err_msg);
}

void print_func_less_parms_err(struct token *fdtok, struct token *fstok, char *name){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s funksiyasina teleb olunan saydan az arqument oturulur(setir %d).\n",fdtok->row, fdtok->col, name, fstok->row );
	add_to_errors(fdtok->row, fdtok->col, err_msg);
}
		
void 	print_func_must_ret_val_err(struct token *tok, char *name, int row){

	memset(err_msg, '\0', ERR_MSG_LEN);
	printf("sehv(%d,%d): %s funksiyasinin netice qaytarmasi teleb olunur(setir %d).\n", tok->row, tok->col, name,  row);
	add_to_errors(tok->row, tok->col, err_msg);
}

void print_dbl_dec_memb_err(struct token *tokk, struct token *tokj, char *name){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s sinfinin %s adli deyishen heddi artiq elan olunub(setir %d).\n", tokk->row, tokk->col, name, tokk->tok,tokj->row);	
	add_to_errors(tokk->row, tokk->col, err_msg);
}
					

void print_token_type_1_err(int row, int col, char *msg){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s .\n", row, col, msg);	
	add_to_errors(row, col, err_msg);
}


void print_token_type_2_err(int row, int col, char *msg, char c){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv:\\%c %s .\n", row, col, c, msg);	
	add_to_errors(row, col, err_msg);
}


void print_token_type_3_err(int row, int col, char *msg, char *s){

	memset(err_msg, '\0', ERR_MSG_LEN);
	sprintf(err_msg, "(%d:%d)sehv: %s %s .\n", row, col, s, msg);	
	add_to_errors(row, col, err_msg);
}


void print_best_errors(int max){

	int i;

	for (i=0; i<ins_tll_cnt; ++i)
		if ( ins_tll_ary[i].decr_cnt == max  ||  is_terminal(ins_tll_ary[i].ins_id)  )
			printf("%s", ins_tll_ary[i].msg );

}

void select_best_errors(int *res){

  int i,k,bef_parse, aft_parse,max, ins_ind;

  tree_copy = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  tree_copy->first = tree_copy->last = NULL;
  tree_copy->count = 0;

  make_copy_of_toklist(tree, tree_copy);

  max = 0;
  ins_ind = 0;

  for (i=0; i<ins_tll_cnt; ++i){

	  insert_tll_next_index( ins_tll_ary[i].ins_pos, ins_tll_ary[i].ins_id);
	  bef_parse = get_tll_count(tree);
      parser_core();
	  aft_parse  = get_tll_count(tree);

	 k = bef_parse - aft_parse;

	 ins_tll_ary[i].decr_cnt = k;

	 if (k > max){
		  max = k;
		  ins_ind = i;
	  }

	  make_copy_of_toklist(tree_copy, tree);
  }

  *res = ins_ind;

  print_best_errors(max);
}



void replace_errors(int i, int j){

	struct sehv *eri, *erj;
	char *ptmp;
	int tmp;

	eri = &sehvler[i];
	erj = &sehvler[j];

	tmp = erj->row;
	erj->row = eri->row;
	eri->row = tmp;

	tmp = erj->col;
	erj->col = eri->col;
	eri->col = tmp;

	ptmp = erj->msg_ptr;
	erj->msg_ptr = eri->msg_ptr;
	eri->msg_ptr = ptmp;
}

void sort_errors(){

	int i,j;

	for (i=0; i<err_cnt-1; ++i)
		for (j=i+1; j<err_cnt; ++j)
			if (sehvler[i].row > sehvler[j].row)
				replace_errors(i,j);
}

 




void print_errors(){


	int i;

	for (i=0; i<err_cnt; ++i)
		printf("%s",  sehvler[i].msg_ptr);

	//printf("sehvler %d : xeberdarliqlar %d.\n", err_cnt, wrn_cnt);

	return ;

	if (err_cnt > 0){

		char cem[5];
	
		if (err_cnt > 1)
			strcpy(cem, "ler");
		else
			strcpy(cem, "");

		printf("Sehv%sinizi duzeldin ve proqrami yeniden kompilyasiya edin.\n", cem);
	}

}

void print_error_ind(int k){

	printf("%s",  ins_tll_ary[k].msg);
}


int is_terminal(int id){

	int i;

	for (i=0; i<term_len; ++i)
		if (terminals[i] == id)
			return 1;

	return 0;
}

int math_oper(int id){

	switch(id){
		case MULT:
		case DEL:
		case NEGSIG:
		case POSSIG:
		case PRCT:
			return 1;
		default:
			return 0;	
	}

}

int math_oprnd(int id){

	switch(id){
		case NUMB:
		case IDT:
		case CRG1:
		case CRG2:
		case FLOAT:
		case EXPR:
		case STRMEM:
			return 1;
		default:return 0;	
	}

}

struct rule_token * get_rule_token(int id){

	int i, ind = -1;

	for (i=0; i<token_len; ++i)
		if (rule_token_set[i].id == id ){
			ind = i;
			break; 
		}

    // found rule set for given token
	if (ind >= 0)
		return &rule_token_set[ind];

	
	//no rule_set for given token
	return NULL;
}


void report_error_for_rule(struct tok_list_el *p_left, struct tok_list_el *p_right, int missing_left, int missing_right){


	if (p_right != NULL){
		sprintf(err_msg, "sehv(%d,%d):", p_right->tok->row, p_right->tok->col);
		sprintf(err_msg, " %s -den sonra ", p_right->tok->tok);
		print_tok_str(missing_right);
		printf(" gozlenirdi. \n");
		return;
	}

	if (p_left != NULL){
		printf("sehv(%d,%d):", p_left->tok->row, p_left->tok->col);
		printf(" %s -den sonra ", p_left->tok->tok);
		print_tok_str(missing_left);
		printf(" gozlenirdi. \n");
		return;
	}

}


int get_id_pos(struct rule *rl, int id){

	int i;

	for (i=0; i<=rl->len; ++i)
		if (rl->left[i] == id)
			return  i;
	
	//printf("Sehv: kompilyator xetasi: zehmet olmasa program metnin ahmed.sadikhov@gmail.com unvanina gonderin. get_id_pos -1 qaytardi\n");
	return -1; //error
}


void scan_backward(struct rule *rl, struct tok_list_el *tll, int pos,  int *match_cnt, int *missing_left,  struct tok_list_el **p_left){

	int i;

	for (i=pos-1; i >= 0; --i){
		tll = tll->prev;
		if (rl->left[i] != tll->id){
			*missing_left = rl->left[i];
			*p_left = tll;
			return;
		}
		(*match_cnt)++;
	}
}

void scan_forward(struct rule *rl, struct tok_list_el *tll, int pos, int *match_cnt,  int *missing_right, struct tok_list_el **p_right){

	int i;

	for (i=pos+1; i <=rl->len; ++i){
		tll = tll->next;
		if (rl->left[i] != tll->id){
			*missing_right = rl->left[i];
			*p_right = tll->prev;
			return;
		}
		(*match_cnt)++;
	}
}


int probe_for_rule(int rule_ind, struct tok_list_el *tll, int *match_cnt, int *missing_left, int *missing_right, struct tok_list_el **p_left, struct tok_list_el **p_right){

	struct rule *rl;	
	int i, cnt, pos;

	rl = &rule_set[rule_ind];

	//get pos of ID in given rule
	pos = get_id_pos(rl, tll->id);

	scan_backward(rl, tll, pos, match_cnt, missing_left, p_left);
	scan_forward(rl, tll, pos, match_cnt, missing_right, p_right);

	return pos;
}

void report_error_for_rule_new( int rule_ind, struct tok_list_el *tll, struct tok_list_el **ptll){

	struct tok_list_el *tmp_tll = tll;
	struct rule *rl;	
	int i, cnt, pos;

	rl = &rule_set[rule_ind];

	char msg[1024];

	//get pos of ID in given rule
	pos = get_id_pos(rl, tll->id);

	for (i=pos + 1;  i < rl->len; ++i){
		tll = tll->next;
		if (rl->left[i] != tll->id )
			break;
	}

	if (i < rl->len){
		//printf("sehv(%d,%d):", tll->prev->tok->row, tll->prev->tok->col);
		//printf(" %s -den sonra ", tll->prev->tok->tok);
		sprint_tok_str(rl->left[i], msg);
		//printf(" gozlenirdi. \n");

		print_after(tll->prev, msg);
		add_ins_tll(tree, tll->prev, rl->left[i]);

		*ptll = tll;
		return;
	}


	tll = tmp_tll;

	for (i=pos - 1; i >= 0; --i){
		tll = tll->prev;
		if (rl->left[i] != tll->id )
			break;
	}

	if ( i >= 0){
		//printf("sehv(%d,%d):", tll->next->tok->row, tll->next->tok->col);
		//printf(" %s -den evvel ", tll->next->tok->tok);
		sprint_tok_str(rl->left[i], msg);
		//printf(" gozlenirdi. \n");

		print_before(tll->next, msg);
		add_ins_tll(tree, tll, rl->left[i]);
	}

	tll = tmp_tll;

	for (i=pos + 1; i<=rl->len; ++i)
		tll = tll->next;

	*ptll = tll->next;

}


void insert_missing_token( int rule_ind, struct tok_list_el *tll, struct tok_list_el **ptll){

	struct tok_list_el *tmp_tll = tll;
	struct rule *rl;	
	int i, cnt, pos;

	rl = &rule_set[rule_ind];

	char msg[1024];

	//get pos of ID in given rule
	pos = get_id_pos(rl, tll->id);

	for (i=pos + 1;  i < rl->len; ++i){
		tll = tll->next;
		if (rl->left[i] != tll->id )
			break;
	}

	if (i < rl->len){
		//printf("sehv(%d,%d):", tll->prev->tok->row, tll->prev->tok->col);
		//printf(" %s -den sonra ", tll->prev->tok->tok);
		//sprint_tok_str(rl->left[i], msg);
		//printf(" gozlenirdi. \n");

		//print_after(tll->prev, msg);
		printf("parse_tree before insert---------------------------\n\n");
		print_tok_list(tree);
		printf("\ninserting ");
		print_tok(rl->left[i]);
		printf("\n");
		//insert_tll_next(tree, tll->prev, rl->left[i] );
		printf("parse_tree after insert---------------------------\n\n");
		print_tok_list(tree);
		

		*ptll = tll;
		return;
	}


	tll = tmp_tll;

	for (i=pos - 1; i >= 0; --i){
		tll = tll->prev;
		if (rl->left[i] != tll->id )
			break;
	}

	if ( i >= 0){
		//printf("sehv(%d,%d):", tll->next->tok->row, tll->next->tok->col);
		//printf(" %s -den evvel ", tll->next->tok->tok);
		sprint_tok_str(rl->left[i], msg);
		//printf(" gozlenirdi. \n");

		//print_before(tll->next, msg);

		printf("parse_tree before insert---------------------------\n\n");
		print_tok_list(tree);
		printf("\ninserting ");
		print_tok(rl->left[i]);
		printf("\n");
		//insert_tll_next(tree, tll, rl->left[i] );
		printf("parse_tree after insert---------------------------\n\n");
		print_tok_list(tree);

	}

	tll = tmp_tll;

	for (i=pos + 1; i<=rl->len; ++i)
		tll = tll->next;

	*ptll = tll->next;

}


void correct_math_errors(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	while (tll->id != FILESON){
	
		if (math_oper(tll->id)){
		
			if (!math_oprnd(tll->prev->id));
			//	insert_tll_next(tll->prev, IDT);

			if (!math_oprnd(tll->next->id));
			//	insert_tll_next(tree, tll, IDT);
		
		}

		tll = tll->next;
	
	}

}

void correct_ary_errors(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	while (tll->id != FILESON){
	
		if (math_oper(tll->id)){
		
			if (!math_oprnd(tll->prev->id))
				;//insert_tll_next(tll->prev, IDT);

			if (!math_oprnd(tll->next->id))
				;//insert_tll_next(tree, tll, IDT);
		
		}

		tll = tll->next;
	
	}

}

void check_errors(){

	struct rule_token *rtn;
	int i,max, match_cnt, found_rule, pos;
	int missing_left, missing_right, found_missing_left, found_missing_right;
	struct tok_list_el *p_left, *p_right, *found_p_left, *found_p_right, *ptll;

	int max_restart_cnt = 0;

   struct tok_list_el *tll = tree->first->next;

   while (tll->id != FILESON){

	   //temporary code
	   if (tll->id == FILEBEG){
		   tll = tll->next;
		   continue;
	   }

	   /* skipped
	   //do not checking errors for non terminals

	   if (!is_terminal(tll->id)){
		   tll = tll->next;
		   continue;
	   }
	   */

   	   rtn = get_rule_token(tll->id);

	   // no rule_set for given token
	   if (rtn == NULL){
		//fixup
		   //printf("sehv:setir %d gozlenilmeyen hedd", tll->tok->row);
		   tll = tll->next;
		   continue;
	   }

	   max = 0;
	   found_rule = 0;
	   found_p_left = found_p_right = NULL;
	   p_left = NULL;
	   p_right = NULL;

	   // probe for best matching syntax rule for given token
	   for (i=0; i<rtn->len; ++i){
		   match_cnt = 0;
		   //printf("rule %d for token ", rtn->rules[i]);
		  // print_tok(tll->id);
		  // printf("\n");
		  // print_rule_id(rtn->rules[i]);
		   pos = probe_for_rule(rtn->rules[i], tll, &match_cnt, &missing_left, &missing_right, &p_left, &p_right);
		   if (match_cnt > max){
			   max = match_cnt;
			   found_rule = i;
			   found_missing_left = missing_left;
			   found_missing_right = missing_right;
			   found_p_left = p_left;
			   found_p_right = p_right;
		   }
	   }

	   if (ESR_DEBUG){
		   printf("\n Best matching rule for token ");
		   print_tok(tll->id);
		   printf(" is \n\n");
		   print_rule_id(rtn->rules[found_rule]);
		   printf("end of best match\n\n");
	   }
	   
	   // report error for best matched syntax rule
	   //report_error_for_rule(p_left, p_right, missing_left, missing_right);
	   report_error_for_rule_new(rtn->rules[found_rule], tll, &ptll);
		
		//tll has to updated inside report_error
		//otherwise we may encounter endless loop 

	   //tll = tll->next;

		tll = ptll;

		/*

		   printf("\nparse_tree before insert---------------------------\n");
		   print_tok_list(tree);

		   if (found_p_left != NULL)
			insert_tll_next(found_p_left, missing_left);
		   if (found_p_right != NULL)
			insert_tll_next(found_p_right, missing_right);

		   printf("\nparse_tree after insert---------------------------\n");
		   print_tok_list(tree);

		   //restating parser
		   parser_core();
		   printf("\nparse tree after restart ==============================\n");
		   print_tree_dx(tree); 
		   if ( ++max_restart_cnt > 3)
			   return;
			   */

 

   }

   //print_ins_tll_ary();
   //find_subtractions();

}

int get_next_tok(int id){

	int i;

	for (i=0; i<err_msg_len; ++i )
		if ( rl_msg_set[i].id == id )
			return  rl_msg_set[i].next_id;

	return -1;
}

struct tok_list_el *get_ins_tll_adrs(struct tok_list *tree, int index){

	struct tok_list_el *tll = tree->first;
	int i;

	for (i=0; i<index; ++i)
		tll = tll->next;

	return tll;
}

//insert new tll on given tll adress
void insert_tll_next(struct tok_list_el *ptll, int id){

	struct tok_list_el *tll;

	get_row_col_right(ptll, &ptll->tok->row, &ptll->tok->col, &ptll->tok->tok);

	tll = create_tok_list_el(ptll->tok);

	tll->id = id;

    //now insert
	tll->next = ptll->next;
	ptll->next = tll;

}


//insert new tll by index pos on tok_list
void insert_tll_next_index(int index, int id){

	struct tok_list_el *tll, *ptll;

	ptll = get_ins_tll_adrs(tree, index);

	get_row_col_right(ptll, &ptll->tok->row, &ptll->tok->col, &ptll->tok->tok);

	tll = create_tok_list_el(ptll->tok);

	tll->id = id;

	if (ESR_DEBUG){

	printf("\ninserting ");
	print_tok(id);
	printf(" after ");
	print_tok(ptll->id);
	printf("\n");
	}

	//now insert
	tll->next = ptll->next;
	ptll->next = tll;

	//print_tok_list(tree);
}


void check_errors_new(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	correct_math_errors();

/*    build_cur_layer_hp_oprs(tree);
	build_cur_layer_lp_oprs(tree);
*/
    correct_ary_errors();

}

void insert_missing(){

	struct rule_token *rtn;
	int i,max, match_cnt, found_rule, pos;
	int missing_left, missing_right, found_missing_left, found_missing_right;
	struct tok_list_el *p_left, *p_right, *found_p_left, *found_p_right, *ptll;

	int max_restart_cnt = 0;

	printf("\n----------------------------------------------\n\n");
	printf("----------------  ERRORS BY RULE ------------------\n\n");
   

   struct tok_list_el *tll = tree->first->next;

   while (tll->id != FILESON){

	   //temporary code
	   if (tll->id == FILEBEG){
		   tll = tll->next;
		   continue;
	   }


	   /* skipped
	   //do not checking errors for non terminals

	   if (!is_terminal(tll->id)){
		   tll = tll->next;
		   continue;
	   }
	   */

   	   rtn = get_rule_token(tll->id);

	   // no rule_set for given token
	   if (rtn == NULL){
		//fixup
		   printf("sehv:setir %d gozlenilmeyen hedd", tll->tok->row);
		   tll = tll->next;
		   continue;
	   }

	   max = 0;
	   found_rule = 0;
	   found_p_left = found_p_right = NULL;
	   p_left = NULL;
	   p_right = NULL;

	   // probe for best matching syntax rule for given token
	   for (i=0; i<rtn->len; ++i){
		   match_cnt = 0;
		   //printf("rule %d for token ", rtn->rules[i]);
		  // print_tok(tll->id);
		  // printf("\n");
		  // print_rule_id(rtn->rules[i]);
		   pos = probe_for_rule(rtn->rules[i], tll, &match_cnt, &missing_left, &missing_right, &p_left, &p_right);
		   if (match_cnt > max){
			   max = match_cnt;
			   found_rule = i;
			   found_missing_left = missing_left;
			   found_missing_right = missing_right;
			   found_p_left = p_left;
			   found_p_right = p_right;
		   }
	   }

	   printf("\n Best matching rule for token ");
	   print_tok(tll->id);
	   printf(" is \n\n");
	   print_rule_id(rtn->rules[found_rule]);
	   printf("end of best match\n\n");
	   
	   // report error for best matched syntax rule
	   //report_error_for_rule(p_left, p_right, missing_left, missing_right);
	   //report_error_for_rule_new(rtn->rules[found_rule], tll, &ptll);
	   insert_missing_token(rtn->rules[found_rule], tll, &ptll);
		
		//tll has to updated inside report_error
		//otherwise we may encounter endless loop 

	   //tll = tll->next;

		tll = ptll;

		/*

		   printf("\nparse_tree before insert---------------------------\n");
		   print_tok_list(tree);

		   if (found_p_left != NULL)
			insert_tll_next(found_p_left, missing_left);
		   if (found_p_right != NULL)
			insert_tll_next(found_p_right, missing_right);

		   printf("\nparse_tree after insert---------------------------\n");
		   print_tok_list(tree);

		   //restating parser
		   parser_core();
		   printf("\nparse tree after restart ==============================\n");
		   print_tree_dx(tree); 
		   if ( ++max_restart_cnt > 3)
			   return;
			   */

 

   }

}



int check_parse_tree(struct tok_list *tree){

  struct tok_list_el *tll = tree->first;

  if (tree->first->id == FILEBEG    && \
      (tree->first->next->id == TEXT || tree->first->next->id == FILESON )&& \
      tree->first->next->next->id == FILESON )
      return 1;  /* clean parsing  */

   


  return 0;
}

void check_errors_2(){

	int k = 0;

	init_prev_links();
	
	check_errors();

	select_best_errors(&k);

	// print_ins_tll_ary();

}


void check_parse_errors(){
 
	struct tok_list_el *tll = tree->first;

  if (tree->first->id == FILEBEG    && \
      (tree->first->next->id == TEXT || tree->first->next->id == FILESON )&& \
      tree->first->next->next->id == FILESON )
      return ;  /* clean parsing  */
 

   do_ast_job();
   sort_errors();
   print_errors();

   //reset error counter for checking next stage of parser errors
   err_cnt = 0;

   check_errors_2();

	printf("Parsing agaci qurulmadi. Metn kodun yoxlayin ve proqrami yeniden kompilyasiya edin. \n");
	//printf("Eger metnin dogru tertib olunduguna eminsinizse onda zehmet olmasa kodu ahmed.sadikhov@gmail.com unvanina gonderin.\n");
	printf("\n--------------------------------------------\n");

  exit(0);
}

void reset_error_counter(){

	err_cnt = 0;
}

void check_symtab_errors(){

   //reset error counter for checking next stage of parser errors
   

   if (err_cnt == 0)
	   return;

   sort_errors();
   print_errors();

   printf("Proqramda sehvler var. Metn kodun yoxlayin ve proqrami yeniden kompilyasiya edin. \n");
	//printf("Eger metnin dogru tertib olunduguna eminsinizse onda zehmet olmasa kodu ahmed.sadikhov@gmail.com unvanina gonderin.\n");
	printf("\n--------------------------------------------\n");

  exit(0);
}