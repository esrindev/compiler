  
  /* GPL  V.3  
  Parser code.
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
*/
#if 1
  
  #include "esrin.h"

  #include <conio.h>

extern char * lines[];

struct parse_tree ptree_holder, *ptree = &ptree_holder ;

int prg_found = 0;

struct tok_list *tree;

#define MAX_HALS 100

struct sec{
  struct tok_list_el *hals[MAX_HALS];
  int size;
} sec_cont, *sec;

char *pt;
int row, col; /* used to format error message */

void init_bottom_layer(struct tok_list *tree, struct token tks[]);
void add_cur_layer(struct tok_list *tree, struct layer *lr);
void init_parse_tree(struct parse_tree  **ptree, struct parse_tree ptree_holder);
void print_parse_tree(struct tok_list *tree);
struct layer *create_cur_layer();
struct tok_list_el *create_tok_list_el(struct token *tok);

/* error.c  */
void  sehv(int say, char *s);
void sehv_tll_first(int say, struct tok_list_el *tll, char *s);

extern void report_to_vartab(struct variable *var, int usage);
extern void print_dec_list(struct dec_list_el dec_list[], int dec_cnt);

extern void execute(struct tok_list *tree);

FILE *fp;

/* parsing sehvleri  */
int psh = 0;


int matchid(int k, int tok){

  return ( k  == tok);

}

/* match against id , nb  */
int match_in(int k){

  return (matchid(k, IDT) || matchid(k, NUMB));
  
 }

/* match against id , ary  */
int match_ia(int k){

  return matchid(k, IDT) || matchid(k, CRG1) || matchid(k, CRG2) ;
  
 }

/* match against id , nb or ary, ary will be implemented in  future  */
int match_ina(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2);
  
 }

/* match against id , nb or ary, ary will be implemented in  future  */
int match_inaf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2)  || matchid(k, FLOAT) ;
  
 }

int match_inas(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, STR);
  
 }

int match_inac(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR);
  
 }


int match_inacf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, FLOAT) ;
  
 }


int match_inacs(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR);
  
 }

int match_inacsf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT) ;
  
 }


int match_inacsfE(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) ;
  
 }

int match_inacsfEF(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) || \
 matchid(k, FCALL1);
  
 }


/*
int match_opr(int k){

}
*/
int match_lp_opr(int k){

  return (matchid(k, POSSIG) ||  matchid(k, NEGSIG)) ;

}


int match_hp_opr(int k){

  return ( matchid(k, MULT) ||  matchid(k, DEL) ||  matchid(k, PRCT)) ;

}

int match_opr(int k){

  return match_lp_opr(k) || match_hp_opr(k);

}

int match_cond(int k){

  return (matchid(k, GRT) ||  matchid(k, LES) ||  matchid(k, BERABER) ||  matchid(k, FERQLI) ||  matchid(k, GRTEQU) ||   matchid(k, LESEQU));
 
  /*	      ||   matchid(k, VE)  ||   matchid(k, VEYA)) ; */

}

int match_tsky(int k){

  return (matchid(k, TAM1) || matchid(k, SIMVOL1) || matchid(k, KESR1) || matchid(k, YNT1));

}

void get_row_col_right(struct tok_list_el *tll, int *row, int *col, char **pt){

  int i;

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

  int i;

  if (tll->cld_cnt == 0){ /* reached far left leave, get row and col  */
    *row = tll->tok->row;
    *col = tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far left branch  */
  get_row_col_left(tll->childs[0], row, col, pt);
}

/* print expected string without surrounding ``  */
void print_before(struct tok_list_el *tll, char *msg){
    get_row_col_left(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -dan evvel %s gozlenirdi.\n", row, col, pt,msg);    
	err_cnt++;
}

void print_after(struct tok_list_el *tll, char *msg){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -dan sonra %s gozlenirdi.\n", row, col, pt,msg);    
	err_cnt++;
}

/* convert PRG { KOD1 } into PRG1 */
int cons_prg1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; 

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
  
  if (matchid(a,PRG) && matchid(b, OPNBLK) && matchid(c,KOD1) && matchid(d,CLSBLK)  ){

    ptr->id = PRG1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->cld_cnt   = 4;
    ptr->lrb = 1; //branch
    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next;
    *tll_ref = ptr;
    (*pos)++;
    return 1;	    
  }
 
  return 0;
}


/* convert FUNKSIYA IDT (...) into FUNK_ELAN */
int cons_funk_elan_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c, k;

    k = 0;

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

  k = 0;
  ptr->next = NULL;


  if (matchid(a,FDECL) && matchid(b,YNT1) &&  matchid(c, CLSBRK))
      goto L3;

  if (matchid(a,FDECL) && matchid(b, CLSBRK))
      goto L2;

       goto out;

  L3:
       ptr->childs[2] = tll->next->next;
       ptr->next  =  tll->next->next->next;
       k+=1;

  L2:


       ptr->id = FUNK_ELAN;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;

       k+= 2;

       ptr->cld_cnt   = k;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
           if (ptr->next == NULL)
                        ptr->next = tll->next->next;
       *tll_ref = ptr;
       (*pos)++;

      return 1;

 out:
  return 0;
}


int check_funk_elan_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);

  if (matchid(a,FDECL)){
    if (!(matchid(b,YNT1)||matchid(b, CLSBRK))){
      printf("Sehv:%d:%d: %s -dan sonra elan veya `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (matchid(b,YNT1)){

      get_row_col_right(tll->next, &row, &col, &pt);

      if (!matchid(c, CLSBRK)){
	printf("Sehv:%d:%d: %s -den sonra `)` gozlenirdi.\n", row, col, pt);
	err_cnt++;
      *tll_ref = tll->next->next;
      return 0;
      }
    }
  }

  return 0;
}


/* convert PRG {  VAR1 KOD1 } into PRG1 */
int cons_prg_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e;

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,PRG) && matchid(b, OPNBLK) && matchid(c,VAR1) && matchid(d,KOD1) && matchid(e, CLSBLK)  ){
       ptr->id = PRG1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->childs[4] = tll->next->next->next->next;
       ptr->cld_cnt   = 5;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next->next;
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_prg_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;
   
  get_row_col_right(tll, &row, &col, &pt);  

  if (matchid(a,PRG)){
    if (!matchid(b, OPNBLK)){
      printf("(%d:%d)sehv: `proqram` acar sozunden sonra `{` gozlenirdi.\n", row, col);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!(matchid(c, VAR1)||matchid(c, KOD1))){
      get_row_col_right(tll->next, &row, &col, &pt);  
      printf("(%d:%d)sehv:  `%s` -den sonra elan ve ya kod gozlenirdi.\n", row, col, pt);
	  err_cnt++;
     *tll_ref = tll->next->next;
      return 0;
    }
    if (matchid(c, VAR1)){

      get_row_col_right(tll->next->next, &row, &col, &pt);  

      if (!(matchid(d, KOD1))){
	printf("(%d:%d)sehv: `%s` -den sonra kod gozlenirdi.\n", row, col, pt);
	err_cnt++;
       *tll_ref = tll->next->next->next;
       return 0;
      }
      else{

	get_row_col_right(tll->next->next->next, &row, &col, &pt);  

	if (!matchid(e, CLSBLK)){
	  printf("(%d:%d)sehv: `%s` -den sonra `}` moterizesi gozlenirdi.\n", row, col, pt);
	  err_cnt++;
	  *tll_ref = tll->next->next->next->next;
	  return 0;
	}
      }
    }
    else{

      get_row_col_right(tll->next->next, &row, &col, &pt);  

      if (!(matchid(d, KOD1))){
	printf("(%d:%d)sehv: `%s` -den sonra `}` gozlenirdi.\n", row, col, pt);
	err_cnt++;
	*tll_ref = tll->next->next;
	return 0;      
      }    
    }
  }

  return 0;
}

typedef  enum {S0, S0E, S1, S1E,  S2, S2E, S3, S3E, S4, S4E, S5} States;
  States state = S0;


int prg_err_machine(int input, States *state){

  switch(*state){
  case S0:
    switch(input){
    case PRG:
      *state = S1;
      break;
    default:
      *state = S0E;
      return 1;
    }break;
  case S1:
    switch(input){
    case OPNBLK:
      *state = S2;
      break;
    default:
      *state = S1E;
      return 1;
    }break;
 case S2:
    switch(input){
    case VAR1:
      *state = S3;
      break;
    case KOD1:
      *state = S4;
      break;
    default:
      *state = S2E;
      return 1;
    }break;
  case S3:
    switch(input){
    case KOD1:
      *state = S4;
      break;
    default:
      *state = S3E;
      return 1;
    }break;
  case S4:
    switch(input){
    case CLSBLK:
      *state = S5;
      return 1;
    default:
      *state = S4E;
      return 1;
    }break;
  }
  return 0;
}
/* not used in this version of compiler  */
void check_prg_error_new(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;
  int input;

  while (tll->id != FILESON){
    input = tll->id;
    if(prg_err_machine(input, &state))
      break;
    tll = tll->next;
  }

  switch(state){
  case S0E:
    return;
  case S1E:
    *tll_ref = tll;
    get_row_col_left(tll, &row, &col, &pt);  
    printf("(%d:%d)sehv: `proqram` acar sozunden sonra `{` gozlenirdi.\n", row, col); 
	err_cnt++;
    return;
  case S2E:
    *tll_ref = tll;
   get_row_col_left(tll, &row, &col, &pt);  
   printf("(%d:%d)sehv:  `%s` -den evvel elan ve ya kod gozlenirdi.\n", row, col, pt);
   err_cnt++;
   return;
  case S3E:
    *tll_ref = tll;
   get_row_col_left(tll, &row, &col, &pt);  
   printf("(%d:%d)sehv: `%s` -den evvel kod gozlenirdi.\n", row, col, pt);
   err_cnt++;
   return;
  case S4E:
    *tll_ref = tll;
   get_row_col_left(tll, &row, &col, &pt); 
   printf("(%d:%d)sehv: `%s` -den evvel `}` moterizesi gozlenirdi.\n", row, col, pt); 
   return;
  case S5:
    *tll_ref = tll->next;
    return;
   }

  return ;
}


/* convert kod :  into KOD1 */
int cons_kod1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,KOD) && matchid(b,COLON)) {
       ptr->id = KOD1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_kod1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,KOD)){
    if (!matchid(b,COLON)){
      printf("(%d:%d)sehv `kod` acar sozunden sonra `:` gozlenirdi.\n",row, col);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
} 

  return 0;
}


/* convert { VAR1 KOD1 } into FUNK_BODY */
int cons_funk2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,OPNBLK) && matchid(b,VAR1) && matchid(c, KOD1) && matchid(d,CLSBLK)){
       ptr->id = FUNK_BODY;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


/* convert FUNK FUNK or FUNKS FUNK  into FUNKS */
int cons_funks_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if ((matchid(a,FUNK) || matchid(a, FUNKS) )&& (matchid(b,FUNK) || matchid(b, FUNKS) )){
       ptr->id = FUNKS;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



/* convert FUNK_ELAN FUNK_BODY into FUNK */
int cons_funk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,FUNK_ELAN) && matchid(b,FUNK_BODY)){
       ptr->id = FUNK;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



/* convert { KOD1 } into FUNK_BODY */
int cons_funk1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,OPNBLK) && matchid(b,KOD1) && matchid(c,CLSBLK)){
       ptr->id = FUNK_BODY;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


 /* konvert PRG1 FUNKS or PRG1 FUNK od PRG1 to PRG2   */
int cons_kod_PRG2(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

 
  if (matchid(a, PRG1) && (matchid(b, FUNKS) || matchid(b, FUNK)))
    goto L2;

 if (matchid(a, PRG1))
   goto L1;

 goto out;

 ptr->next = NULL;

 L2:
    ptr->childs[1] = tll->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next; 
 L1:

       ptr->id  = PRG2;
       ptr->childs[0] = tll;

	   k+= 1;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
			ptr->next = tll->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:
  return 0;
}


 /* konvert FILEBEG STRUKT2 or NULL  PRG2 FILESON   to S   */
int cons_kod_S(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,YENI_TIP)||matchid(a,SINIF_TYPE)||matchid(a,STRUKT2))&&matchid(b,PRG2))
    goto L2;

if (matchid(a, PRG2))
   goto L1;

 goto out;

 ptr->next = NULL;

 L2:
    ptr->childs[1] = tll->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next; 
 L1:

       ptr->id  = START;
       ptr->childs[0] = tll;
       k+= 1;
       ptr->cld_cnt   = k;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:
  return 0;
}



/* convert kod : KODDATA into KOD1 */
int cons_kod_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,KOD) && matchid(b,COLON) && matchid(c,KODDATA)){
       ptr->id = KOD1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


/* convert { koddata } into koddata */
int cons_block_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,OPNBLK) && matchid(b,KODDATA) && matchid(c,CLSBLK)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



/* convert koddata koddata into koddata */
int cons_koddata_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if ( !matchid(a, YOXSA) &&  matchid(b,KODDATA) && matchid(c,KODDATA)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll->next;
       ptr->childs[1] = tll->next->next; 
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       ptr->next = tll->next->next->next; 
       tllp->next->next = ptr;

       *tll_ref = ptr;
        (*pos)++;

      return 1;
   }
  return 0;
}


/* convert eger_exp koddata yoxsa koddata into koddata */
int cons_eger_yoxsa_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,EGER_EXP2) && matchid(b,KODDATA) &&  matchid(c,YOXSA) &&  matchid(d,KODDATA) ){
       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int check_eger_yoxsa_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,EGER_EXP2)){
    if (!matchid(b,KODDATA)){
      printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (matchid(c,YOXSA)){
      get_row_col_right(tll->next->next, &row, &col, &pt);   
      if (!matchid(d,KODDATA)){
	printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0	;
      }
    }
  } 

  return 0;
}


/* convert neqederki koddata into koddata */
int cons_neqederki_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,NEQEDERKI2) && matchid(b,KODDATA)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_neqederki_error(struct tok_list_el **tll_ref){
  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,NEQEDERKI2)){
      if (!matchid(b,KODDATA)){
	printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	err_cnt++;
	*tll_ref = tll->next;
	return 0;
      }
    } 

  return 0;
}


/* convert eger_exp koddata into koddata */
int cons_eger_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,EGER_EXP2) && matchid(b,KODDATA) && !matchid(c, YOXSA)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



/* convert  koddata into koddata */
int cons_secim_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g,k;   

  k = 0;


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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,SEC1) && match_ia(b) && matchid(c, CLSBRK)       &&  \
      matchid(d, OPNBLK) &&  matchid(e, HAL2) &&  matchid(f, CLSBLK)){

       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->childs[4] = tll->next->next->next->next;
       ptr->childs[5] = tll->next->next->next->next->next;
       ptr->cld_cnt   = 6;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next =  tll->next->next->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

       return 1;
      }
 
  return 0;
}


int check_secim_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e,f;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,SEC1)){
    if (!matchid(b,IDT)){
      printf("(%d:%d)sehv: `%s` -den sonra deyishen gozlenirdi.\n",row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c, CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s`-den sonra  `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;      
    }
    if (! matchid(d, OPNBLK)){
      get_row_col_right(tll->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `{` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0; 
    }
    if (!matchid(e, HAL2)){
      get_row_col_right(tll->next->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `hal` acar sozu  gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next->next;
      return 0;       
    }
    if (!matchid(f, CLSBLK)){
      get_row_col_right(tll->next->next->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `}`  gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next->next->next;
      return 0;      
    }
  }

  return 0;
}



/* convert dovr_exp koddata into koddata */
int cons_dovr_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,DOVR_EXP) && matchid(b,KODDATA)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_dovr_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,DOVR_EXP)){
    if (!matchid(b,KODDATA)){
      printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}


/* convert NEQEDERKI1 SHERT ) tekraret into NEQEDERKI2  */
int cons_neqederki1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,NEQEDERKI1) && matchid(b,SHERT) && matchid(c,CLSBRK) && matchid(d,TEKRARET )){
       ptr->id = NEQEDERKI2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;

       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}

int check_neqederki1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,NEQEDERKI1)){
    if (!matchid(b,SHERT)){
      printf("(%d:%d)sehv: `%s` -den sonra  shert gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;
    }
    if (!matchid(d,TEKRARET)){
      get_row_col_right(tll->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `tekraret` acar sozu gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0;
    }
  }

  return 0;
}

/* convert EGEREXP1 SHERT ) into EGER_exp2  */
int cons_eger_exp1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,EGER_EXP1) && matchid(b,SHERT) && matchid(c,CLSBRK) && matchid(d,ONDA )){
       ptr->id = EGER_EXP2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;

       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next;
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_eger_exp1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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
 
  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,EGER_EXP1)){
    if (!matchid(b,SHERT)){
      printf("(%d:%d)sehv: `%s` -den sonra shert gozlenirdi.\n",row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;      
    }
    if (!matchid(d,ONDA)){
      get_row_col_right(tll->next->next, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `onda` acar sozu  gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0;      
    }
  }

  return 0;
}


/* convert FE1 , FE1  into FE1  */
int cons_fe1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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
 
  if (matchid(a, FE1) && matchid(b, VERGUL) && matchid(c, FE1)){
       ptr->id = FE1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
      (*pos)++;

    return 1;
   }
  return 0;
}



/* convert () into EMPBRK  */
int cons_emptybrk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if (matchid(a,OPNBRK) && matchid(b,CLSBRK)){
       ptr->id = EMPBRK;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
      (*pos)++;

    return 1;
   }
  return 0;
}


/* convert { } into EMPTYOP  */
int cons_emptyop_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if (matchid(a,OPNBLK) && matchid(b,CLSBLK)){
       ptr->id = EMPTYOP;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
      (*pos)++;

    return 1;
   }
  return 0;
}


int cons_davamet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,DAVAMET) && matchid(b,NOQTEV)){
       ptr->id = DAVAMET1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int check_davamet_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAVAMET)){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -den sonra `;` gozlenirdi.\n", row, col, pt);
	err_cnt++;
    *tll_ref = tll->next;
   }

  return 0;
}



int cons_dayan_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,DAYAN) && matchid(b,NOQTEV)){
       ptr->id = DAYAN1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_dayan_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAYAN)){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -den sonra `;` gozlenirdi.\n", row, col, pt);
	err_cnt++;
    *tll_ref = tll->next;
   }

  return 0;
}



int cons_neqederki_exp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,NEQEDERKI) && matchid(b,OPNBRK)){
       ptr->id = NEQEDERKI1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_neqederki_exp_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

    if (matchid(a,NEQEDERKI)){
      get_row_col_right(tll, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `(` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
   }

  return 0;
}



/* convert eger ( into EGER_exp1  */
int cons_eger_exp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,EGER) && matchid(b,OPNBRK)){
       ptr->id = EGER_EXP1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int check_eger_exp_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

    if (matchid(a,EGER)){
      get_row_col_right(tll, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `(` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
   }

  return 0;
}



int cons_dovr_exp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g,h,i,j,k,l,m; 

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next!=NULL)
    h = tll->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next!=NULL)
    i = tll->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next->next!=NULL)
    j = tll->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next->next->next!=NULL)
    k = tll->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

 if (tll->next->next->next->next->next->next->next->next->next->next->next!=NULL)
    l = tll->next->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

 if (tll->next->next->next->next->next->next->next->next->next->next->next->next!=NULL)
    m = tll->next->next->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,DOVR)){
   if (matchid(b, OPNBRK)){
     if (match_ia(c) || matchid(c, STRMEM)){
     if (matchid(d, ARTSIN) || matchid(d, AZALSIN) ){
       if (match_ina(e) || matchid(e, STRMEM) || matchid(e, EXPR)  ){
       if(matchid(f, NEGSIG)){
        if(matchid(g,DEN) || matchid(g,DAN)){
	  if(match_ina(h) || matchid(h, STRMEM) || matchid(h, EXPR) ){
	  if(matchid(i, NEGSIG)){
	   if(matchid(j,AA)||matchid(j,EE)||matchid(j,YA)||matchid(j,YE)){
	    if (matchid(k,KIMI)){
	     if(matchid(l, CLSBRK)){
	      if(matchid(m, TEKRARET)){
ptr->id = DOVR_EXP;
ptr->childs[0] = tll;
ptr->childs[1] = tll->next;
ptr->childs[2] = tll->next->next;
ptr->childs[3] = tll->next->next->next;
ptr->childs[4] = tll->next->next->next->next;
ptr->childs[5] =  tll->next->next->next->next->next;
ptr->childs[6] =  tll->next->next->next->next->next->next;
ptr->childs[7] =  tll->next->next->next->next->next->next->next;
ptr->childs[8] =  tll->next->next->next->next->next->next->next->next;
ptr->childs[9] =  tll->next->next->next->next->next->next->next->next->next;
ptr->childs[10] = tll->next->next->next->next->next->next->next->next->next->next;
ptr->childs[11]=tll->next->next->next->next->next->next->next->next->next->next->next;
ptr->childs[12]=tll->next->next->next->next->next->next->next->next->next->next->next->next;
ptr->cld_cnt   = 13;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next->next->next->next->next->next->next->next->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	      return 1;
		    }
		   }
		  }
	  	 }
	        }
	       }
	      }
	     }
            }
           }
          }
         }
        }
  return 0;
}


int check_dovr_exp_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e,f,g,h,i,j,k,l,m; 

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next!=NULL)
    h = tll->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next!=NULL)
    i = tll->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next->next!=NULL)
    j = tll->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next->next->next->next->next->next!=NULL)
    k = tll->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

 if (tll->next->next->next->next->next->next->next->next->next->next->next!=NULL)
    l = tll->next->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

 if (tll->next->next->next->next->next->next->next->next->next->next->next->next!=NULL)
    m = tll->next->next->next->next->next->next->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,DOVR)){

    if (!matchid(b, OPNBRK)){
      print_before(tll, "`(`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!(match_ia(c) || matchid(c, STRMEM))){
      print_before(tll->next, "deyishen");
      *tll_ref = tll->next->next;
      return 0;
    }

    if (!(matchid(d, ARTSIN) || matchid(d, AZALSIN))){
      print_before(tll->next->next, "`azalsin`");
      *tll_ref = tll->next->next->next;
      return 0;
    }
    if (!(match_ina(e) || matchid(e, STRMEM) || matchid(e, EXPR) )){
      print_before(tll->next->next->next, "ifade");
      *tll_ref = tll->next->next->next->next;
      return 0;
    }
    if(!matchid(f, NEGSIG)){
      print_before (tll->next->next->next->next, "`-`");
      *tll_ref = tll->next->next->next->next->next;
      return 0;      
    }
    if(!(matchid(g,DEN) || matchid(g,DAN))){
      print_before (tll->next->next->next->next->next, "`den`(`dan`)");
      *tll_ref = tll->next->next->next->next->next->next;
      return 0;      
    }
    if(!(match_ina(h) || matchid(h, STRMEM) || matchid(h, EXPR) )){
      print_before(tll->next->next->next->next->next->next->next, "ifade");
      *tll_ref = tll->next->next->next->next->next->next->next;
      return 0;      
    }
    if(!matchid(i, NEGSIG)){
      print_before(tll->next->next->next->next->next->next->next, "`-`");
      *tll_ref = tll->next->next->next->next->next->next->next->next;
      return 0;
    }
    if(!(matchid(j,AA)||matchid(j,EE)||matchid(j,YA)||matchid(j,YE))){
      print_before (tll->next->next->next->next->next->next->next->next, "`a`(`e`,`ya`,`ye`)");
      *tll_ref = tll->next->next->next->next->next->next->next->next->next;
      return 0;
    }
    if (!matchid(k,KIMI)){
      print_before(tll->next->next->next->next, "`kimi` acar sozu");
      *tll_ref = tll->next->next->next->next->next->next->next->next->next->next;
      return 0;      
    }
    if(!matchid(l, CLSBRK)){
      print_before(tll->next->next->next->next->next->next->next->next->next->next, "`)`");
      *tll_ref = tll->next->next->next->next->next->next->next->next->next->next->next;
      return 0; 
    }
    if(!matchid(m, TEKRARET)){
      print_before(tll->next->next->next->next->next->next->next->next->next->next->next, "`)`");
      *tll_ref = tll->next->next->next->next->next->next->next->next->next->next->next->next;
      return 0;      
    }
  }

  return 0;
}



int cons_asgn_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if ((match_ia(a)||matchid(a, STRMEM)) && matchid(b,ASGN) && (match_inacf(c)||matchid(c, EXPR) || matchid(c, FCALL1)|| matchid(c, SNMEM) || matchid(c, STRMEM)) && matchid(d,NOQTEV)){
    ptr->id  = ASGN_OP;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->cld_cnt   = 4;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_asgn_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(b,ASGN)){
    if (!(match_ia(a)||matchid(a, STRMEM))){
      print_before(tll->next, "deyishen");
      *tll_ref = tll->next->next;
      return 0;
    }
    if ((!match_inacf(c)||matchid(c, EXPR) || matchid(c, FCALL1)|| matchid(c, SNMEM) || matchid(c, STRMEM))){
      print_after(tll->next->next, "ifade");
      *tll_ref = tll->next->next->next;
      return 0;
    }
    if (!matchid(d,NOQTEV)){
      print_before(tll->next->next->next, "`;`");
      *tll_ref = tll->next->next->next->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,CAPET2)&&matchid(b,CLSBRK)&&matchid(c, NOQTEV)){
    ptr->id  = CAPET3;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;

    
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_capet3_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,CAPET2)){
    if (!matchid(b,CLSBRK)){
      print_before(tll, "`)`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,NOQTEV)){
      print_before(tll->next, "`;`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,CAPET1) && matchid(b,VDEC)){
    ptr->id  = CAPET2;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next =  tll->next->next;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int cons_daxilet3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,DAXILET2)&&matchid(b,CLSBRK)&&matchid(c,NOQTEV)){
    ptr->id  = DAXILET3;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

    return 1;
 }

  return 0;
}


int check_daxilet3_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,DAXILET2)){
    if (!matchid(b,CLSBRK)){
      print_before(tll, "`)`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,NOQTEV)){
      print_before(tll->next, "`;`");
      *tll_ref = tll->next->next;
      return 0;       
    }    
  }

  return 0;
}


int cons_daxilet2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,DAXILET1)&&matchid(b,VDEC)){
    ptr->id  = DAXILET2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

    return 1;
 }

  return 0;
}


int cons_capet1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,CAPET1)&& (match_inacsfE(b) || matchid(b, STRMEM) )){
    ptr->id  = CAPET2;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}

int cons_strmem_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; 

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;


  if ((match_ia(a) && matchid(b,DOT) && match_ia(c) && !(matchid(d, DOT) || matchid(d, ARYOPN)  )) || \
      (match_ia(a) && matchid(b,DOT) && matchid(c,STRMEM))){
       ptr->id = STRMEM;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch


       ptr->next =  tll->next->next->next; /* God save me from pointers   */
       *tll_ref = ptr;
       tllp->next = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_strmem_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;

  if (matchid(b,DOT)){
    if (!match_ia(a)){
      print_before(tll, "deyishen");
      *tll_ref = tll->next;
      return 0;
    }
    if (!( match_ia(c)||matchid(c,STRMEM) || matchid(c, SNMEM) || matchid(c, FCALL1))){
      print_before(tll->next, "deyishen");
      *tll_ref = tll->next->next;
      return 0;      
    }
}

  return 0;
}


int cons_snmem_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; 

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;


  if (match_ia(a) && matchid(b,DOT) && (matchid(c, FCALL1)||matchid(c,SNMEM))){
       ptr->id = SNMEM;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch


       ptr->next =  tll->next->next->next; /* God save me from pointers   */
       *tll_ref = ptr;
       tllp->next = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int cons_strukt2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,k;   

  k = 0;


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

  if (matchid(a,STRUKT1)&& matchid(b,YNT2) && matchid(c,CLSBLK) && matchid(d, NOQTEV))
   goto L4;

  if (matchid(a,STRUKT1)&& matchid(b, CLSBLK) && matchid(c,NOQTEV))
   goto L3;

  if (matchid(a, STRUKT2) && matchid(b, STRUKT2))
    goto L2;

  goto out;

  k = 0;
  ptr->next = NULL;

 L4:
    ptr->childs[3] = tll->next->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next->next; 

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = STRUKT2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       k+= 2;
       ptr->cld_cnt   = k;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:

  return 0;
}


int cons_daxilet1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,DAXILET1)&& (match_ia(b) || matchid(b, STRMEM))){
    ptr->id  = DAXILET2;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int check_daxilet1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,DAXILET1)){
    if (!(match_ia(b) || matchid(b, STRMEM))){
      print_after(tll, "deyishen");
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if (matchid(a,CAPET)&&matchid(b,OPNBRK)){
    ptr->id  = CAPET1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_capet_error(struct tok_list_el **tll_ref){


  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,CAPET)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
 }

  return 0;
}




int cons_hal1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;


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

  if (matchid(a,HAL)&& (matchid(b,NUMB) || matchid(b, CHR)) && matchid(c, COLON)){
    ptr->id  = HAL1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;
    
    return 1;
  }
  
  return 0;
}


int check_hal1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,HAL)){
    if (!(matchid(b,NUMB) || matchid(b, CHR))){
      print_after(tll, "EDED ve ya SIMVOL");
      *tll_ref = tll->next;
      return 0;
    }
    else
      if (!matchid(c, COLON)){
	print_after(tll->next, "`:`");
	*tll_ref = tll->next->next;
	return 0;
	}
  }

    /* if (matchid(a, HAL1)  && matchid(b, HAL1)  NOTE: did not checked! */

    if (matchid(a, SUSMA)){
      print_after(tll, "`:`");
      *tll_ref = tll->next;
      return 0;
    }

  return 0;
}



int cons_sus1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,SUSMA)&& matchid(b, COLON)){
    ptr->id  = SUS1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;
       
    return 1;
  }
  
  return 0;
}


int check_sus1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SUSMA)){
     if (!matchid(b, COLON)){
	print_after(tll, "`:`");
	*tll_ref = tll->next;
	return 0;
	}
  }

  return 0;
}



int cons_hal1s_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if ((matchid(a,HAL1) || matchid(a, HAL1S))&& (matchid(b, HAL1)||matchid(b, HAL1S))){
    ptr->id  = HAL1S;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;
       
    return 1;
  }
  
  return 0;
}



int cons_hal2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if (((matchid(a,HAL1) || matchid(a, HAL1S) || matchid(a, SUS1)) && \
       matchid(b,KODDATA) && (matchid(c, CLSBLK) || matchid(c, HAL2))) || \
      (matchid(a, HAL2) && matchid(b, HAL2))){
    ptr->id  = HAL2;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_hal2_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,HAL1) || matchid(a, HAL1S) || matchid(a, SUS1))) { 
    if (!matchid(b, KODDATA)){
      print_after(tll, "emeliyyat ");
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}

int cons_hal3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if ((matchid(a, HAL2) && matchid(b, HAL2)) || \
      (matchid(a, HAL2) && matchid(b, HAL3)) || \
      (matchid(a, HAL3) && matchid(b, HAL3)) || \
      (matchid(a, HAL3) && matchid(b, HAL2))){
    ptr->id  = HAL3;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int cons_sec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if (matchid(a,SEC)&&matchid(b,OPNBRK)){
    ptr->id  = SEC1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_sec_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,SEC)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
 }

  return 0;
}

/* convert  FCALL IDT into FCALL VDEC  */
int cons_vdec1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a, FCALL) && (match_inacsfEF(b) || matchid(b, STRMEM))){
    ptr->id  = VDEC;
    ptr->childs[0] = tll->next;
    ptr->cld_cnt   = 1;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}

/* convert , idt into VDEC  */
int cons_vdec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  if (!matchid(a, IDT) &&
      ((match_inacsfE(b)||matchid(b,FCALL1)||matchid(b,STRMEM)||matchid(b,VDEC))) &&\
        matchid(c, VERGUL) &&\
      ((match_inacsfE(d)||matchid(d,FCALL1)||matchid(d,STRMEM) || matchid(d, VDEC)))){
    ptr->id  = VDEC;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->childs[2] = tll->next->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* conevrt funksiya idt ( into FDECL  */
int cons_fdecl_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,     int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (matchid(a,FUNKSIYA) && matchid(b,IDT) && matchid(c, OPNBRK) ){
    ptr->id  = FDECL;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
	ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_fdecl_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,FUNKSIYA)){
    if (!matchid(b,IDT)){
      print_after(tll, "funksiyanin adi");
      *tll_ref = tll->next;
      return 0;	
    }
    else{
      if (!matchid(c, OPNBRK)){
      print_before(tll->next, "`(`");
      *tll_ref = tll->next->next;
      return 0;		
      }
    }
  }

  return 0;
}


/* conevrt idt ( into FCALL  */
int cons_fcall_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,     int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (!matchid(a,FUNKSIYA) && matchid(b,IDT) && matchid(c, OPNBRK)){
    ptr->id  = FCALL;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}




int cons_strukt1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (matchid(a,STRUKT)&&matchid(b,IDT)&&matchid(c,OPNBLK)){
    ptr->id  = STRUKT1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* convert SINIF IDT to SINIF_HEAD_START  */
int cons_SHS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF)&&matchid(b,IDT)){
    ptr->id  = SINIF_HEAD_START;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}

int check_SHS_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,SINIF)){
    if (!matchid(b,IDT)){
      print_after(tll, "sinfin adi");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,OPNBLK)){
      print_after(tll->next, "`{`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}


/* convert SINIF_head_START OPNBLK to SINIF_HEAD  */
int cons_SH_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF_HEAD_START)&&matchid(b,OPNBLK)){
    ptr->id  = SINIF_HEAD;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}




/* convert  SINIF_HEAD YNT || FUNK  into SINIF_HEAD SINIF_BODY 
   !!! not consuming SINIF_HEAD  */
int cons_SB_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (((matchid(a,SINIF_HEAD)||matchid(a,COLON))&&matchid(b,FUNK))||\
      ((matchid(a,SINIF_HEAD)||matchid(a,COLON))&&matchid(b,YNT2))){

       ptr->id  = SINIF_BODY;
       ptr->childs[0] = tll->next;
       ptr->cld_cnt   = 1;
       ptr->lrb = 1; //branch

       ptr->next = tll->next->next; 
       tllp->next->next = ptr;
       *tll_ref = ptr;
    (*pos)++;
    return 1;
}
 
  return 0;
}


int check_SB_1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,SINIF_HEAD)||matchid(a,COLON))){
    if(!(matchid(b,FUNK)||matchid(b,YNT2))){
      print_after(tll, "elan ve ya funksiya");
      *tll_ref = tll->next;
      return 0;
    }
  }    

  return 0;
}


/* convert SINIF_BODY YNT2 || FUNK to SINIF_BODY  */
int cons_SBS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF_BODY)&&(matchid(b,SINIF_BODY)||matchid(b,YNT2)||matchid(b, FUNK))){

       ptr->id  = SINIF_BODY;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
  }
  return 0;
}




/* convert  SINIF_HEAD SINIF_BLOK into SINIF_HEAD SINIF_DECL
   inserting ficticious MEXSUSI tll as fist child SPECIAL CASE
   in order to make all members private by default. 
   !!! not consuming SINIF_HEAD  */
int cons_SHD_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  struct tok_list_el *fictiv_gizli;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_BODY)){
    ptr->id  = SINIF_DECL;
    fictiv_gizli = create_tok_list_el(tll->tok); /*that may confuse parser, possible BUG*/ ;
    fictiv_gizli->id  = MEXSUSI;
    ptr->childs[0] = fictiv_gizli;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* convert QORUNAN || MEXSUSI || SERBEST  COLON SINIF_BODY  to SINIF_DECL  */
int cons_SD_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if ((matchid(a,QORUNAN)||matchid(a,MEXSUSI)||matchid(a,SERBEST))&&\
      matchid(b, COLON) && matchid(c,SINIF_BODY)){
    ptr->id  = SINIF_DECL;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int check_SD_1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if ((matchid(a,QORUNAN)||matchid(a,MEXSUSI)||matchid(a,SERBEST))){
    if (!(matchid(b, COLON) || matchid(b, SINIF_BODY))){
      print_after(tll, "`:`");
      *tll_ref = tll->next;
      return 0;
    } 
    if (matchid(b, COLON)){
      if (!(matchid(c, SINIF_BODY)||matchid(c, FUNK)||matchid(c, YNT2))){
	print_after(tll->next, "elan ve ya kod");
	*tll_ref = tll->next->next;
	return 0;	
      }
    }
  }

  return 0;
}


/* convert SINIF_DECL  SINIF_DECL  to SINIF_DECL  */
int cons_SDS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF_DECL)&&matchid(b,SINIF_DECL)){
    ptr->id  = SINIF_DECL;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* convert SINIF_HEAD SINIF_DECL CLSBLK NOQTEV to SINIF_TYPE  */
int cons_ST_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref, int *pos){

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
 
  if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_DECL) && matchid(c, CLSBLK) && matchid(d,NOQTEV)){
    ptr->id  = SINIF_TYPE;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

    return 1;
 }

  return 0;
}


int check_ST_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

   if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_DECL)){
     if(!matchid(c, CLSBLK)){
       print_after(tll, "`}`");
       *tll_ref = tll->next;
       return 0;
     }

    if(!matchid(d, NOQTEV)){
      print_after(tll->next, "`;`");
       *tll_ref = tll->next->next;
       return 0;
     }
   }

  return 0;
}



int cons_daxilet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,DAXIL)&&matchid(b,OPNBRK)){
    ptr->id  = DAXILET1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int check_daxilet_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAXIL)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
 }

  return 0;
}



/* consume   var1 tam2   into var1   */
int cons_var1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,VAR1) && (matchid(b,TAM2)||matchid(b,KESR2)||matchid(b,SIMVOL2) || matchid(b, YNT2))){
    ptr->id  = VAR1; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* consume   var :  into var1   */
int cons_var_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,VAR) && matchid(b,COLON)){
    ptr->id  = VAR1; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int cons_ynt2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;

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

  if (matchid(a, YNT1) &&  matchid(b, YNTS) && matchid(c, NOQTEV))
   goto L3;

  if ((matchid(a, YNT1) && matchid(b, NOQTEV)) || \
      (matchid(a, YNT2) && matchid(b, YNT2)))
   goto L2;

 goto out;

      k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = YNT2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:

  return 0;
}



int cons_ynt1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a, YNT1) && matchid(b,VERGUL) && matchid(c, YNT1)){
       ptr->id  = YNT1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


/* IDT IA -> YNT1  */
int cons_ynt_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;


  if (matchid(a,IDT) && match_ia(b)){
    ptr->id  = YNT1;
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

  return 0;
}




/* YNT1 VERGUL IA -> YNT1 YNTS  */
int cons_ynts_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  /* a test ucun istifade olunur, parse olunmur  */
  if (matchid(a,YNT1) && matchid(b, VERGUL) && match_ia(c)){
    ptr->id  = YNTS;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch


    ptr->next = tll->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* YNTS VERGUL IA -> YNTS  */
int cons_ynts1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  /* a test ucun istifade olunur, parse olunmur  */
  if (matchid(a,YNTS) && matchid(b, VERGUL) && match_ia(c)){
    ptr->id  = YNTS;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch


    tllp->next = ptr;
    ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}






/* consume (SINIF_TYPE | STRUKT2 | YENI_TIP)  (SINIF_TYPE | STRUKT2 | YENI_TIP)   into YENI_TIP   */
int cons_yeni_tip_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,SINIF_TYPE) ||matchid(a,STRUKT2) || matchid(a,YENI_TIP)) && \
      (matchid(b,SINIF_TYPE) ||matchid(b,STRUKT2) || matchid(b,YENI_TIP))){
    ptr->id  = YENI_TIP;/* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* consume (FCALL1 || SNMEM)  NOQTEV to KODDATA   */
int cons_fcall1_op_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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


  if ((matchid(a,FCALL1)||matchid(a, SNMEM)) && matchid(b,NOQTEV)){
    ptr->id  = KODDATA;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}

int check_fcall1_op_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;


  if ((matchid(a,FCALL1)||matchid(a, SNMEM))){
    if (!matchid(b,NOQTEV)){
      print_after(tll, "`;`");
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}


/* consume  SHERT ve SHERT into SHERT   */
int cons_vveya_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 if (matchid(a,SHERT)&&(matchid(b,VE)||matchid(b,VEYA))&&matchid(c,SHERT)){
    ptr->id  = SHERT;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


int check_vveya_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if ((matchid(b,VE)||matchid(b,VEYA))){
    if (!matchid(a,SHERT)){
      print_before(tll->next, "shert");
      *tll_ref = tll->next->next;
      return 0;
    }
    if (!matchid(c,SHERT)){
      print_after(tll->next, "shert");
      *tll_ref = tll->next->next->next;
      return 0;
    }
  }

  return 0;
}

/* cons  x > y into SHERT */
int cons_cond_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if ((match_inacf(a)|| matchid(a, EXPR) || matchid(a, FCALL1) || matchid(a, STRMEM)) && match_cond(b) && (match_inacf(c)|| matchid(c, EXPR)  || matchid(c, FCALL1) || matchid(c, STRMEM) )){
    ptr->id  = SHERT; 
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


int check_cond_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (match_cond(b)){
    if (!(match_inacf(a)|| matchid(a, EXPR) || matchid(a, FCALL1) || matchid(a, STRMEM))){
	print_before(tll->next, "ifade");
	*tll_ref = tll->next->next;
	return 0;
      }
      if (!(match_inacf(c)|| matchid(c, EXPR)  || matchid(c, FCALL1) || matchid(c, STRMEM))){
	print_after(tll->next, "ifade");
	*tll_ref = tll->next->next->next;
	return 0;
      }
  }            

  return 0;
}

/* consume (x) into x, i.e. get rid of brackets when there is only one 
   element inside   */
int cons_brk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  /* SPECIAL CASE !!!!!!!!!!!!!!!!

     ONLY here we do not consume sequence into tree but simply remove
     brackets from sequence like (x) -> x .

   */

  if (matchid(a,OPNBRK)&&(match_ina(b)|| matchid(b, EXPR) || matchid(b, FCALL1) || matchid(b, SNMEM) || matchid(b, STRMEM) ||  matchid(b,SHERT))&&matchid(c,CLSBRK)){
    /* normally we have to delete a and c from memory as they are no more needed  */

       tllp->next = tll->next;
       *tll_ref = tll->next;
       tll->next->next = tll->next->next->next;
       (*pos)++;
    return 1;
 }

  return 0;
}

/* consume on declaration   */
int cons_crg2_dec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g;   
  int d1, d2;

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,IDT) && \
      matchid(b,ARYOPN) && matchid(c,NUMB) && matchid(d,ARYCLS) &&	\
      matchid(e,ARYOPN) && matchid(f,NUMB) && matchid(g,ARYCLS) ){

    ptr->id  = CRG2; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->childs[4] = tll->next->next->next->next;
    ptr->childs[5] = tll->next->next->next->next->next;
    ptr->childs[6] = tll->next->next->next->next->next->next;
    ptr->cld_cnt   = 7;

    ptr->lrb = 1; //branch

    d1 = atoi(tll->next->next->tok->tok);
    d2 = atoi(tll->next->next->next->next->next->tok->tok);

    tll->tok->d1 = d1;
    tll->tok->d2 = d2;

    /* report to vartab  */
    /*add_dec_table(&dec_tab, tll->tok, CRG2);*/

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* consume on declaration  */
int cons_crg1_dec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref, int *pos){

  struct tok_list_el *tllp = *tll_ref;
  int d1;

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
 

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

  if (matchid(a,IDT) && matchid(b,ARYOPN) && matchid(c, NUMB) && matchid(d,ARYCLS)){
    ptr->id  = CRG1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       d1 = atoi(tll->next->next->tok->tok);
       tll->tok->d1 = d1;

       /*add_dec_table(&dec_tab, tll, CRG1);*/

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

    return 1;
 }

  return 0;
}



/* consume on kod  */
int cons_crg2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,IDT) && \
      matchid(b,ARYOPN) && (match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM)) && matchid(d,ARYCLS) &&	\
      matchid(e,ARYOPN) && (match_ina(f)|| matchid(f, EXPR)  || matchid(f, STRMEM)) && matchid(g,ARYCLS) ){ 

    ptr->id  = CRG2; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->childs[4] = tll->next->next->next->next;
    ptr->childs[5] = tll->next->next->next->next->next;
    ptr->childs[6] = tll->next->next->next->next->next->next;
    ptr->cld_cnt   = 7;

    ptr->lrb = 1; //branch

    /* report to vartab  */
    /*add_usg_table(&usg_tab, tll->tok, CRG2);*/

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int check_crg2_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e,f,g;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

      if (matchid(b,ARYOPN)){
	if (!matchid(a,IDT)){
	  print_before(tll->next, "deyishen");
	  *tll_ref = tll->next->next;
	  return 0;
	}
	if (!(match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM))){
	  print_after(tll->next, "ifade");
	  *tll_ref = tll->next->next->next;
	  return 0;
	}
	if (!matchid(d,ARYCLS)){
	  print_after(tll->next->next, "]");
	  *tll_ref = tll->next->next->next->next;
	  return 0;
	}
	if (matchid(e,ARYOPN)){
	  if (!(match_ina(f)|| matchid(f, EXPR)|| matchid(f, STRMEM))){
	  print_after(tll->next->next->next->next, "ifade");
	  *tll_ref = tll->next->next->next->next->next;
	  return 0;
	  }
	  if (!matchid(g,ARYCLS)){
	  print_after(tll->next->next->next->next->next, "`]`");
	  *tll_ref = tll->next->next->next->next->next->next;
	  return 0;
	  }
	}
      }

  return 0;
}



int cons_crg1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

 if (matchid(a,IDT) && matchid(b,ARYOPN) && (match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM)) && matchid(d,ARYCLS)){
    ptr->id  = CRG1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}



int check_crg1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;
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

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

 if (matchid(a,IDT) && matchid(b,ARYOPN)){
   if (!(match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM))){
     print_after(tll->next, "ifade");
     *tll_ref=tll->next->next;
     return 0;
   }
   else{
     if (!matchid(d,ARYCLS)){
       print_after(tll->next->next, "ifade");
       *tll_ref=tll->next->next->next;
       return 0;       
     }
   }
 }

  return 0;
}




int cons_qaytar_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;


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

  if (matchid(a, QAYTAR) &&  (match_inacsfE(b) || matchid(b, STRMEM)) && matchid(c, NOQTEV))
   goto L3;

 if (matchid(a, QAYTAR) && matchid(b, NOQTEV))
   goto L2;

 goto out;

      k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = QAYTAR1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:

  return 0;
}



int check_qaytar_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,k;   

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

  if (matchid(a, QAYTAR)){
    if (!(match_inacsfE(b) || matchid(b, STRMEM) || matchid(b, NOQTEV))){
      print_after(tll, "deyishen ve ya `;`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(b, NOQTEV)){
      print_after(tll, "`;`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}




int cons_fcall1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;


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

 if (matchid(a, FCALL) &&  matchid(b, VDEC) && matchid(c, CLSBRK))
   goto L3;

 if (matchid(a, FCALL) && matchid(b, CLSBRK))
   goto L2;

 goto out;

      k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = FCALL1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:

  return 0;
}



int cons_plus_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (!match_hp_opr(a) && ((match_inaf(b)|| matchid(b, FCALL1) || matchid(b, EXPR) || matchid(b, STRMEM)) && match_lp_opr(c) && (match_inaf(d)|| matchid(d, FCALL1) ||matchid(d, EXPR) || matchid(d, STRMEM))) && !match_hp_opr(e) ){
    ptr->id  = EXPR;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->childs[2] = tll->next->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch
    
    /* WARNING!!!!!!
       Only here we copy tok of first child to parent, in order to
       be able to get tok on interp.c/get_head_tok().
       Otherwise we NEVER copy anything from child to parent,
       as this may break parsing logic.
    */
    ptr->tok = ptr->childs[0]->tok;
    
    /* special case: we don't consume all nodes as childs but border
       nodes checked for hp oprs, hope that works   */
    
    ptr->next = tll->next->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;
    
    
    return 1;
  }
  
  return 0;
}



int cons_mult_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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


  if ((match_inaf(a)||  matchid(a, FCALL1) || matchid(a, EXPR) || matchid(a, STRMEM)   ) && match_hp_opr(b) && (match_inaf(c)||  matchid(c, FCALL1) || matchid(c, EXPR)  || matchid(c, STRMEM)  )){
    ptr->id  = EXPR;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}




int cons_convert_tsk_fe1(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a, IDT) && match_ia(b)){
    ptr->id  = FE1; /* convertin ti koddata  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}




int cons_convert_kod_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a, DAXILET3)  ||  matchid(a, CAPET3)  || \
      matchid(a, ASGN_OP)   ||  matchid(a, EMPTYOP) || \
      matchid(a, DAVAMET1)  ||  matchid(a, DAYAN1)  || \
      matchid(a, QAYTAR1)){
    ptr->id  = KODDATA; /* convertin ti koddata  */
    ptr->childs[0] = tll;
    ptr->cld_cnt   = 1;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next; 
    *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}



void convert_cur_layer_tsk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
// 
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_convert_tsk_fe1(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;

  }

}




void convert_cur_layer_kod (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 // tll = tree->count; /* how many elemnts 'r one current layer  */
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_convert_kod_tok(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;

  }

}




void  convert_to_fe1_tsk(struct tok_list *tree){
  
	 convert_cur_layer_tsk(tree);
}



void  convert_to_koddata_dca(struct tok_list *tree){

	convert_cur_layer_kod(tree);
}




int build_cur_layer_fe1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_fe1_tok(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_funk_elan (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int    pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funk_elan_tok(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;
   }
 
  return pos;
}


int check_funk_elan_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_funk_elan_error(&tll);

    tll = tll->next;
   }
 
  return 1;
}


int build_cur_layer_funks (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funks_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_funk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funk_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_funk_body (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funk1_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_funk2_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_prg (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_kod_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_kod1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_prg_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_prg1_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



 int check_prg_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_prg_error(&tll);

   tll = tll->next;
   }
 
  return 1;
}


int build_cur_layer_sinif (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

       if (cons_SHS_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_SH_tok(ptr, &tll,   &pos))
      continue; 
    
   if (cons_SB_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SBS_tok(ptr, &tll,   &pos))
    continue; 

   
   tll = tll->next;
   }
 
  return pos;
}



int check_sinif_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_SHS_error(&tll);
    check_SB_1_error(&tll);
    check_SD_1_error(&tll);
    check_ST_error(&tll);

   tll = tll->next;
   }
 
  return 1;
}

int build_cur_layer_sinif_dec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

 
      if (cons_SHD_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SD_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SDS_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_ST_tok(ptr, &tll,   &pos))
     continue; 
   
   tll = tll->next;
   }
 
  return pos;
}




int build_cur_layer_PRG2 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_kod_PRG2(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}


int build_cur_layer_S (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_kod_S(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}


int build_cur_layer_koddata (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_dovr_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_eger_yoxsa_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_eger_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_neqederki_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_hal2_tok(ptr, &tll,   &pos))
      continue; 

   //   if (cons_hal3_tok(ptr, &tll,   &pos))
   //   continue; 

  if (cons_secim_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_koddata_tok(ptr, &tll,   &pos))
      continue;

    if (cons_block_tok(ptr, &tll,   &pos))
      continue;

  tll = tll->next;

   }

  return pos;
}


int check_koddata_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_dovr_error(&tll);
    check_eger_yoxsa_error(&tll);
    check_neqederki_error(&tll);
    check_hal2_error(&tll);
    check_secim_error(&tll);

  tll = tll->next;

   }

  return 1;
}


int build_cur_layer_dax1_cap1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_daxilet1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_daxilet2_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_capet1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_capet2_tok(ptr, &tll,   &pos))
      continue;

   if (cons_hal1s_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }

  return pos;
}



int check_dax1_cap1_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_daxilet1_error(&tll);

    /*
    if (cons_daxilet1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_daxilet2_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_capet1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_capet2_tok(ptr, &tll,   &pos))
      continue; 
    */

   tll = tll->next;
   }

  return 1;
}


int build_cur_layer_dax_cap (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */


    if (cons_daxilet_tok(ptr, &tll, &pos))
      continue; 


     if (cons_capet_tok(ptr, &tll,  &pos))
      continue; 


    /* egeri de burda eliyiriy  */
     if (cons_eger_exp_tok(ptr, &tll,  &pos))
      continue;

     if (cons_neqederki_exp_tok(ptr, &tll,  &pos))
      continue;

    if (cons_hal1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_sus1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_sec_tok(ptr, &tll,  &pos))
      continue;  

    if (cons_davamet_tok(ptr, &tll, &pos))
      continue; 


     if (cons_dayan_tok(ptr, &tll,  &pos))
      continue; 


    /* emptyop de burda eliyiriy  */
     if (cons_emptyop_tok(ptr, &tll,   &pos))
      continue;


         if (cons_fdecl_tok(ptr, &tll,  &pos))
      continue;


     if (cons_fcall_tok(ptr, &tll,  &pos))
      continue;

     /*
      if (cons_emptybrk_tok(ptr, &tll,   &pos))
      continue;
     */

   tll = tll->next;

  }

  return pos;
}

void check_dax_cap_errors(struct tok_list *tree){


  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  while (tll!=NULL){

     check_daxilet_error(&tll);
     check_capet_error(&tll);
     check_eger_exp_error(&tll);
     check_neqederki_exp_error(&tll);
     check_hal1_error(&tll);
     check_sus1_error(&tll);
     check_sec_error(&tll);
     check_davamet_error(&tll);
     check_dayan_error(&tll);
     //     check_emptyop_tok(&tll); NOT implemented
     check_fdecl_error(&tll);
     /*

     if (cons_emptyop_tok(ptr, &tll,   &pos)) note omplemented.

     if (cons_fcall_tok(ptr, &tll,  &pos))
     */

   tll = tll->next;
 }
   
}


int build_cur_layer_simple_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_daxilet3_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_capet3_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_asgn_tok(ptr, &tll,   &pos))
      continue; 


  if (cons_dovr_exp_tok(ptr, &tll,   &pos))
      continue; 

  /* shertin consumunda sintaksis telebi olan moterezelerde consume\
     olundugundan eger 2 fazada cons olunur daxil ve capet operatorlari kimi  */
 if (cons_eger_exp1_tok(ptr, &tll,   &pos))
      continue; 

 if (cons_neqederki1_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_fcall1_op_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_qaytar_tok(ptr, &tll,   &pos))
      continue; 


   tll = tll->next;
  }

  return pos;
}



int check_simple_oprs_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_daxilet3_error(&tll);
    check_capet3_error(&tll);
    check_asgn_error(&tll);
    check_dovr_exp_error(&tll);
    check_eger_exp1_error(&tll);
    check_neqederki1_error(&tll);
    check_fcall1_op_error(&tll);
    check_qaytar_error(&tll);

    tll = tll->next;
  }

  return 1;
}



int build_cur_layer_crg_dec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_crg2_dec_tok(ptr, &tll,  &pos))
      continue; 
     if (cons_crg1_dec_tok(ptr, &tll,  &pos))
      continue; 
    
   tll = tll->next;
  }

  return pos;
}



int build_cur_layer_yeni_tip (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_yeni_tip_tok(ptr, &tll,  &pos))
      continue; 
    
   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_ynt (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

   if (cons_ynt_tok(ptr, &tll,   &pos))
      continue; 
   if (cons_ynt1_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;

  }

  return pos;
}




int build_cur_layer_ynts (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

   if (cons_ynts_tok(ptr, &tll,   &pos))
      continue; 
   if (cons_ynts1_tok(ptr, &tll,   &pos))
      continue; 
  if (cons_ynt2_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;

  }

  return pos;
}



int build_cur_layer_vars (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

    if (cons_strukt1_tok(ptr, &tll,   &pos))
      continue;

   if (cons_strukt2_tok(ptr, &tll,   &pos))
      continue;

  if (cons_var_tok(ptr, &tll,   &pos))
      continue;
  if (cons_var1_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;

  }

  return pos;
}


int build_cur_layer_vdec1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_vdec1_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_vdec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_vdec_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_fcall1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_fcall1_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_lp_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_plus_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}



int build_cur_layer_hp_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_crg1_tok(ptr, &tll,   &pos))
      continue; /* match found, cons_emel updated everything 
                   and copied new element to upper layer  */
   if (cons_crg2_tok(ptr, &tll,   &pos))
      continue;
   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_strmem_tok(ptr, &tll,   &pos))
      continue;
   if (cons_mult_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}



int build_cur_layer_snmem (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_snmem_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_cnds (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_cond_tok(ptr, &tll,   &pos))
      continue;
   if (cons_vveya_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}



int check_cnds_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  
  while (tll!=NULL){

    check_cond_error(&tll);
    check_vveya_error(&tll);

    /*
   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_cond_tok(ptr, &tll,   &pos))
      continue;
   if (cons_vveya_tok(ptr, &tll,   &pos))
      continue;
    */

   tll = tll->next;
  }

  return 1;
}



void bottom_up_parse_fe1(struct tok_list *tree){
 
    while(build_cur_layer_fe1(tree));
}


void bottom_up_parse_snmem(struct tok_list *tree){
 
    while(build_cur_layer_snmem(tree));
}


void bottom_up_parse_funk_elan(struct tok_list *tree){

 
    while(build_cur_layer_funk_elan(tree));
}


void bottom_up_parse_yeni_tip(struct tok_list *tree){

 
    while(build_cur_layer_yeni_tip(tree));
}



void bottom_up_parse_funk_body(struct tok_list *tree){
 
    while(build_cur_layer_funk_body(tree));
}


void bottom_up_parse_funks(struct tok_list *tree){
 
    while(build_cur_layer_funks(tree));
}


void bottom_up_parse_funk(struct tok_list *tree){


    while(build_cur_layer_funk(tree));
}


void bottom_up_parse_prg(struct tok_list *tree){
 
    while(build_cur_layer_prg(tree));

}

void bottom_up_parse_PRG2(struct tok_list *tree){
 
    while(build_cur_layer_PRG2(tree));
}

void bottom_up_parse_S(struct tok_list *tree){
  /* do not use while loop  */
    build_cur_layer_S(tree);
}


void bottom_up_parse_koddata(struct tok_list *tree){

 
    while(build_cur_layer_koddata(tree));
    
}

void bottom_up_parse_dax1_cap1(struct tok_list *tree){
 
    while(build_cur_layer_dax1_cap1(tree));    
}

void bottom_up_parse_dax_cap(struct tok_list *tree){

	while(build_cur_layer_dax_cap(tree));    
}


/* array declaration 1 and 2 size  */
void bottom_up_parse_crg_dec(struct tok_list *tree){

      while(build_cur_layer_crg_dec(tree));    
}

void bottom_up_parse_vars(struct tok_list *tree){
 
    while(build_cur_layer_ynt(tree));    
    while(build_cur_layer_ynts(tree));    
    while(build_cur_layer_vars(tree));    

    
}


void bottom_up_parse_exprs(struct tok_list *tree){
 

    int k;

 loop:

    k = build_cur_layer_hp_oprs(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_lp_oprs(tree);
    if (k>0)
      goto loop;

    k = build_cur_layer_fcall1(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec1(tree);
    if (k>0) 
      goto loop;



}


void check_exprs_erros(struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;

  while (tll!=NULL){
 
    //  check_crg1_error(&tll);
    check_crg2_error(&tll);
    check_strmem_error(&tll);
    /*

   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_strmem_tok(ptr, &tll,   &pos))
      continue;
   if (cons_mult_tok(ptr, &tll,   &pos))
      continue;
-----
    if (cons_plus_tok(ptr, &tll,   &pos))
-----
    if (cons_fcall1_tok(ptr, &tll,   &pos))
-----
    if (cons_vdec_tok(ptr, &tll,   &pos))
-----
    if (cons_vdec_tok(ptr, &tll,   &pos))

    */

    /*
    k = build_cur_layer_hp_oprs(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_lp_oprs(tree);
    if (k>0)
      goto loop;

    k = build_cur_layer_fcall1(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec1(tree);
    if (k>0) 
      goto loop;


    */
    tll = tll->next;
  }
}


void bottom_up_parse_cnds(struct tok_list *tree){
 
    while(build_cur_layer_cnds(tree));
    
}


void bottom_up_parse_sinif(struct tok_list *tree){
 
    while(build_cur_layer_sinif(tree));
    while(build_cur_layer_sinif_dec(tree));
    
}


void bottom_up_parse_simple_oprs(struct tok_list *tree){
 
    while(build_cur_layer_simple_oprs(tree));
    
}


/* checking code comes here  */

void add_sec_hal(struct sec *sec, struct tok_list_el *tll){

  sec->hals[sec->size] = tll;
  sec->size++;

}

void build_secim(struct tok_list_el *tll, struct sec *sec){

  int i;

  if (tll->id == HAL1){
    if (tll->childs[0]->id == HAL){
      add_sec_hal(sec, tll->childs[1]);
      return;
    }
    if (tll->childs[0]->id == SUSMA){
      add_sec_hal(sec, tll->childs[0]);
    return ;
    }
}

  for (i=0; i<tll->cld_cnt; ++i)
    build_secim(tll->childs[i], sec);

  return;
}

void check_secim(struct sec *sec){

  struct tok_list_el *tll;
  int i, j,  sus, hits, cnt = sec->size;

  unsigned long hals[MAX_HALS];

  sus = -1;
  hits = 0;

  for (i=0; i<cnt; ++i){
    tll = sec->hals[i];
    if (tll->id == SUSMA){
      if (sus > 0)
	printf("Sehv: setir %d: sec operatoru susma hali evvel setir %d -de elan olunmushdur.\n", tll->tok->row, sec->hals[sus]->tok->row);
	  err_cnt++;
      sus = i;
      continue;
    }

    if (tll->id != NUMB && tll->id != CHR){
      printf("Sehv: setir %d: sec operatoru hal parametri olaraq yalniz tam tipli eded ve ya simvol verile biler.\n ", tll->tok->row);
	  err_cnt++;
      continue;
    }

    hals[hits] = tll->tok->hid;

    for (j = 0; j<hits; ++j)
      if (hals[j] == hals[hits]){
	printf("Sehv: setir %d: sec operatoru hal parametrinin \'%s\' qiymeti artiq daxil olunub (setir %d).\n", sec->hals[hits]->tok->row, sec->hals[hits]->tok->tok, sec->hals[j]->tok->row);
	err_cnt++;
	break;
      }
    
    hits++;
    
  }
}



int check_parse_tree(struct tok_list *tree){

  struct tok_list_el *tll = tree->first;

  if (tree->first->id == FILEBEG    && \
      tree->first->next->id == START && \
      tree->first->next->next->id == FILESON )
    return 1;  /* clean parsing  */

   
printf("Sizin %d sehvleriniz var. Sehvleri duzeldin ve proqrami yeniden kompilyasiya edin.\n", err_cnt);
exit(1);


  return 0;
}



void check_brk_cnt(struct tok_list_el *tll, int d_op, int s_op){

  int i;

  if (tll->cld_cnt > 0){

    if (tll->childs[0]->id == DOVR_EXP )
      d_op = 1;

    if (tll->childs[0]->id == SEC1 )
      s_op = 1;
  }
    
    if (tll->id == DAYAN && !(d_op || s_op)){
    printf("Sehv: setir %d: \'dayan\' operatoru dovr ve ya secim daxilinde elan olunmayib\n", tll->tok->row);
    psh++;
  }
    
  if (tll->id == DAVAMET && !d_op){
    printf("Sehv: setir %d: \'davamet\' operatoru dovr daxilinde elan olunmayib\n", tll->tok->row);
    psh++;
  }
  
    
  for (i=0; i<tll->cld_cnt; ++i)
    check_brk_cnt(tll->childs[i], d_op, s_op);

}

void check_prg_kwd(struct tok_list *tree){

  struct tok_list_el *tll = tree->first;

  while (tll != NULL){
    if (tll->id == PRG){
	prg_found = 1;
	break;
    }

    tll = tll->next;
  }

  if (!prg_found){
    printf("Sehv: \"proqram\" acar sozu tapilmadi.\n");
  }
}

int parse(){

  int res = 0;

  tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  tree->first = tree->last = NULL;
  tree->count = 0;

  init_bottom_layer_new(tree);

  /* bashlamazdan evvele proqram acar sozunu yoxla  */
  check_prg_kwd(tree);

  bottom_up_parse_dax_cap(tree);  
   check_dax_cap_errors(tree);

  bottom_up_parse_crg_dec(tree); 
  // no check for the moment

  bottom_up_parse_vars(tree);
  // check_vars_errors(tree);

  bottom_up_parse_exprs(tree);
   check_exprs_erros(tree);

  bottom_up_parse_cnds(tree);
   check_cnds_errors (tree);

  bottom_up_parse_snmem(tree);
  bottom_up_parse_dax1_cap1(tree); 
   check_dax1_cap1_errors(tree);

  bottom_up_parse_simple_oprs(tree);
  check_simple_oprs_errors(tree); 

  convert_to_koddata_dca(tree);
  //    convert_to_fe1_tsk(tree);  
  //  bottom_up_parse_fe1(tree);

  bottom_up_parse_funk_elan(tree); 
   check_funk_elan_errors(tree);

  bottom_up_parse_koddata(tree);
   check_koddata_errors (tree);  

  bottom_up_parse_prg(tree);
   check_prg_errors(tree);

  bottom_up_parse_funk_body(tree);
  bottom_up_parse_funk(tree);

  bottom_up_parse_sinif(tree);
   check_sinif_errors (tree);   

  bottom_up_parse_yeni_tip(tree);
  bottom_up_parse_funks(tree);
  bottom_up_parse_PRG2(tree);
  bottom_up_parse_S(tree);

      // print_tree_dx(tree); 
  /* !!!!!!!!!!!!  Parsing Finish !!!!!!!!!!   */
     

	 return 0;
}

 
#endif
