/* GPL  V.3  
  Esrin Interpretator Main Module
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
*/

  

#include "esrin.h"

extern void build_exec_pre_list(struct band_list_el *bll, struct tok_list *plst);
extern void build_pre_list(struct band_list_el *bll, struct tok_list *plst);
extern void print_pre_list(struct tok_list plst);
extern void print_parse_tree(struct parse_tree *ptree);
extern void push_to_stek(struct stek *stk, void *dptr);
extern void *pop_from_stek(struct stek *stk);
extern void init_funcs_ret_addrs(struct band_list_el band[], int size);

struct band_list_el  exec_band[BAND_SIZE] ;
int band_size, prg_start;

int cnt_flg, brk_flg;

/* input bufer, input bufer pointer  */
char ib[MAX_INB_LEN], *ibp;

/* prevent endless loop  */
int elp = 0;

  struct var_table *glbmt;

int exec_asgn(struct band_list_el *bll);


unsigned char *start_adrs(int lgm){

  struct func_table_el *fptr;
  unsigned char *adrs;

  switch(lgm){
  case LOCAL:
    adrs = (unsigned char *)fnstk.top->dptr;
    break;
  case MEMBER:
    adrs = (unsigned char *)obstk.top->dptr;
    break;
  case GLOBAL:
    adrs = glbmt->area;
    break;
  }

  return adrs;
}


unsigned char *head_adrs(struct band_list_el *head){

  struct func_table_el *fptr;
  unsigned char *adrs;
  int lgm ,ofst;

  ofst = head->ofst;
  lgm = head->lgm;

  switch(lgm){
  case LOCAL:
    adrs = (unsigned char *)fnstk.top->dptr;
    break;
  case MEMBER:
    adrs = (unsigned char *)obstk.top->dptr;
    break;
  case GLOBAL:
    adrs = glbmt->area;
    break;
  }

  return adrs;
}





unsigned char *get_address(struct band_list_el *bll){

  unsigned char *adrs;

  adrs = start_adrs(bll->lgm);

  return adrs + bll->ofst;
}


void set_val(unsigned char *adrs, long double value, int tip){

  union val{
    char c;
    long int li;
    long double ld;
  } v;

   switch(tip){
    case 1:
      v.li = (long int)value;
      memcpy(adrs, &v, sizeof(long int));
    break;
   case 2:
      v.ld = (long double)value;
      memcpy(adrs, &v, sizeof(long double));
    break;
   case 3:
      v.c = (char)value;
      memcpy(adrs, &v, sizeof(char));
   }
}




int exec_asgn(struct band_list_el *bll){

  unsigned char *adrs;
  long double val;
  void *ptr;
  int tip;

  ptr = pop_from_stek(&adrstk);
  adrs = (unsigned char *)ptr;
  /* !!! do not free ptr popped from adrstk   */ 

  ptr = pop_from_stek(&valstk);
  val = *(long double *)ptr;
  free(ptr);

  tip = bll->tip;

  set_val(adrs, val, tip);

  return 0;
}


int exec_numb(struct band_list_el *bll){

  long double val, *ptr;

  val = (long double)atoi(bll->tok);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);

  return 0;
}


int exec_artsin(struct band_list_el *bll){

  long double *ptr;

  ptr = (long double *)malloc(sizeof(long double));
  *ptr = 1;
  push_to_stek(&valstk, ptr);

  return 0;
}


int exec_azalsin(struct band_list_el *bll){

  long double *ptr;

  ptr = (long double *)malloc(sizeof(long double));
  *ptr = 0;
  push_to_stek(&valstk, ptr);

  return 0;
}


int exec_float(struct band_list_el *bll){

  long double val, *ptr;

  val = (long double)atof(bll->tok);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);

  return 0;
}



int exec_expr(struct band_list_el *bll){

  long double v, v1, v2, *ptr;

  ptr = (long double *)pop_from_stek(&valstk);
  v2 = *ptr;
  free(ptr);

  ptr = (long double *)pop_from_stek(&valstk);
  v1 = *ptr;
  free(ptr);

  switch(bll->id){
  case POSSIG: 
    v = v1 + v2;  
    break;
  case NEGSIG: 
    v = v1 - v2;  
    break;
  case MULT:   
    v = v1 * v2;  
    break;
  case DEL:    
    if (v2 == 0){
      printf("0-ra bolme emeliyyati, program icrasin dayandirdi.\n");
      exit(0);
    }
    v = v1 / v2;  
    break;
  case PRCT:   
    if (v2 == 0){
      printf("0-ra bolme emeliyyati, program icrasin dayandirdi.\n");
      exit(0);
    }
    v = (long int)v1 % (long int)v2;  
    break;
  }

  ptr = (long double *)malloc(sizeof(long double));
  *ptr = v;
  push_to_stek(&valstk, ptr);

  return 0;
}



int exec_shert(struct band_list_el *bll){

  long double v, v1, v2, *ptr;

  ptr = (long double *)pop_from_stek(&valstk);
  v2 = *ptr;
  free(ptr);

  ptr = (long double *)pop_from_stek(&valstk);
  v1 = *ptr;
  free(ptr);

    v = 0; /* condition does not met  */

  switch(bll->id){
  case GRT: 
    if (v1 > v2)
      v = 1;  
    break;
  case LES: 
    if (v1 < v2)
      v = 1;  
    break;
  case GRTEQU:   
    if (v1 >= v2)
      v = 1;  
    break;
  case LESEQU:    
    if (v1 <= v2)
      v = 1;  
    break;
  case BERABER:   
    if (v1 == v2)
      v = 1;  
    break;
  case FERQLI:   
    if (v1 != v2)
      v = 1;  
    break;
  case VE:   
    if (v1 &&  v2)
      v = 1;  
    break;
  case VEYA:   
    if (v1 || v2)
      v = 1;  
    break;
  }

  ptr = (long double *)malloc(sizeof(long double));
  *ptr = v;
  push_to_stek(&valstk, ptr);

  return 0;
}


int exec_idt_adrs(struct band_list_el *bll){

  unsigned char *adrs;
  
  adrs = get_address(bll);
  push_to_stek(&adrstk, adrs);

  return 0;
}

int exec_chr(struct band_list_el *bll){


  long double *ptr;
  char val;


  val = *(bll->tok);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = (long double)val;
  push_to_stek(&valstk, ptr);

  return 0;
}

int exec_str_adrs(struct band_list_el *bll){

  push_to_stek(&adrstk, bll->tok);

  return 0;
}


int exec_idt_val(struct band_list_el *bll){
  
  unsigned char *adrs;
  long double *ptr, val;
  int tip, size;

  tip = bll->tip;
  size = bll->size;

  adrs = get_address(bll);
  val =  get_value(adrs, tip, size);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);

  return 0;
}



int exec_idt_ofst(struct band_list_el *bll){
  
  unsigned char *adrs;

  adrs = (unsigned char *)pop_from_stek(&adrstk);
  adrs += bll->ofst;
  push_to_stek(&adrstk, adrs);

  return 0;
}

int exec_crg1_adrs(struct band_list_el *bll){

  long double val, *ptr;
  unsigned char *adrs;
  int ind, size, max_row;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);

  max_row = bll->d1;
  ind = (int)val;
  if (ind <1 || ind > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind, max_row);
     exit(0);
  }
  
  size = bll->size;
  adrs = get_address(bll);
  adrs += (ind - 1)*size;
  push_to_stek(&adrstk, adrs);

  return 0;
}

int exec_crg1_val(struct band_list_el *bll){

  long double val, *ptr;
  unsigned char *adrs;
  int ind, tip, size, max_row;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);

  max_row = bll->d1;
  ind = (int)val;
  if (ind <1 || ind > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind, max_row);
     exit(0);
  }
  
  tip = bll->tip;
  size = bll->size;
  adrs = get_address(bll);
  adrs += (ind - 1)*size;
  
  val  = get_value(adrs, tip, size);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);  

  return 0;
}


int exec_crg1_ofst(struct band_list_el *bll){

  long double val, *ptr;
  unsigned char *adrs;
  struct token *tok;
  int ind, tip, size, max_row;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);

  max_row = bll->d1;
  ind = (int)val;
  if (ind <1 || ind > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind, max_row);
     exit(0);
  }

  tip = bll->tip;
  size = bll->size;
  adrs = (unsigned char *)pop_from_stek(&adrstk);  
  adrs += (ind - 1)*size;
  push_to_stek(&adrstk, adrs);

  return 0;
}

int exec_crg2_adrs(struct band_list_el *bll){



  long double val, *ptr;
  unsigned char *adrs;
  struct token *tok;
  int ind1, ind2, tip, size, max_row, max_col;

  max_row = bll->d1;
  max_col = bll->d2;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind2 = (int)val;

  if (ind2 <1 || ind2 > max_col ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind2, max_col);
     exit(0);
  }

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind1 = (int)val;

  if (ind1 <1 || ind1 > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind1, max_row);
     exit(0);
  }
  
  tip = bll->tip;
  size = bll->size;
  adrs = get_address(bll);

  adrs += ((ind1 - 1)* max_row + ind2 - 1)*size;
  push_to_stek(&adrstk, adrs);

  return 0;
}

int exec_crg2_val(struct band_list_el *bll){


  long double val, *ptr;
  unsigned char *adrs;
  int ind1, ind2, tip, size, max_row, max_col;

  max_row = bll->d1;
  max_col = bll->d2;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind2 = (int)val;

  if (ind2 <1 || ind2 > max_col ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind2, max_col);
     exit(0);
  }

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind1 = (int)val;

  if (ind1 <1 || ind1 > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind1, max_row);
     exit(0);
  }
  
  tip = bll->tip;
  size = bll->size;
  adrs = get_address(bll);

  adrs += ((ind1 - 1)* max_row + ind2 - 1)*size;

  val  = get_value(adrs, tip, size);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);  

  return 0;
}



int exec_crg2_ofst(struct band_list_el *bll){



  long double val, *ptr;
  unsigned char *adrs;
  struct token *tok;
  int ind1, ind2, tip, size, max_row, max_col;

  max_row = bll->d1;
  max_col = bll->d2;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind2 = (int)val;

  if (ind2 <1 || ind2 > max_col ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind2, max_col);
     exit(0);
  }

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ind1 = (int)val;

  if (ind1 <1 || ind1 > max_row ){
     printf("Setir %d: Cergenin serhedden kenara cixma xetasi %s, ind = %d, max_row = %d\n",  bll->row, bll->tok, ind1, max_row);
     exit(0);
  }
  
  tip = bll->tip;
  size = bll->size;
  adrs = (unsigned char *)pop_from_stek(&adrstk);  
  adrs += ((ind1 - 1)* max_row + ind2 - 1)*size;
  push_to_stek(&adrstk, adrs); 

  return 0;
}



int exec_strmem_val(struct band_list_el *bll){

  long double val, *ptr;
  unsigned char *adrs;
  struct token *tok;
  int tip, size;

  tip = bll->tip;
  size = bll->size;

  adrs = (unsigned char *)pop_from_stek(&adrstk);
  val = get_value(adrs, tip, size);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);
  
  return 0;
}

void print_str(char *str){

  int i,j,k,pos;
  char *tmp, *ptr;

  tmp = (char *)malloc(5); /* ele bele  */

      ptr = str;
      j=0;
      pos = 0;
      k = strlen(ptr);
      free(tmp);
      tmp = (char *)malloc(k);
      while(j<k){
	pos = j;
	while((j<k)&&(*(ptr+j)!='\\')) 
	  j++;
	strncpy(tmp, ptr+pos, j-pos);
	*(tmp+j-pos) = '\0';
	printf("%s",tmp);
	if (*(ptr+j)=='\\'){
	  char a,b;
	  a = *(ptr+j+1);
	  switch(a){
	case 'y':
	  printf("\n");
	      j+=2;
	  break;
	case 't':
	  printf("\t");
	      j+=2;
	  break;
	case '\\':
	  printf("\\");
	      j+=2;
	  break;
	case '\"':
	  printf("\"");
	      j+=2;
	  break;
	case '\'':
	  printf("\'");
	      j+=2;
	  break;
	  default:
	    printf("\\");
	    j++;
	  }
	}
      }
}

/* the same as exec_numb ,but here we push values to glbstk  */
int exec_fargs_numb(struct band_list_el *bll){

  long double val, *ptr;

  val = (long double)atoi(bll->tok);
  ptr = (long double *)malloc(sizeof(long double));
  *ptr = val;
  push_to_stek(&valstk, ptr);

  return 0;
}


int exec_capet_numb(struct band_list_el *bll){

  long double  *ptr, val;
  long int ival;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  ival = (long int)val;
  printf("%d",ival);

  return 0;
}


int exec_capet_float(struct band_list_el *bll){

  long double  *ptr, val;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  printf("%llf",val);

  return 0;
}


int exec_capet_chr(struct band_list_el *bll){

  long double  *ptr, val;
  char c;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  c = (char)val;
  printf("%c",c);

  return 0;
}

int exec_capet_idt(struct band_list_el *bll){

  long double  *ptr, val;
  struct token *tok;

  /*
  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);
  */

  switch(bll->tip){
  case 1:
    exec_capet_numb(bll);
    break;
  case 2:
    exec_capet_float(bll);
    break;
  case 3:
    exec_capet_chr(bll);
    break;
  }

  return 0;
}


int exec_capet_crg1(struct band_list_el *bll){

  exec_crg1_val(bll);
  exec_capet_idt(bll);

  return 0;
}


int exec_capet_crg2(struct band_list_el *bll){

  exec_crg2_val(bll);
  exec_capet_idt(bll);

  return 0;
}


int exec_capet_strmem(struct band_list_el *bll){

  exec_strmem_val(bll);
  exec_capet_idt(bll);

  return 0;
}



int exec_capet_expr(struct band_list_el *bll){

  long double  *ptr, val, rem;
  long int ival;

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);

  ival = (long int)val;
  rem = val - ival;

  if (rem == 0)
    printf("%ld", ival);
  else
    printf("%llf",val);

  return 0;
}


int exec_capet_chr_val(char *s){

  char c, d;

  c = *s;
  
  if (c == '\\') {
    d = *(s + 1);
    switch(d){
    case 'y':
      printf("\n");
      break;
    case 't':
      printf("\t");
      break;
    case '\\':
      printf("\\");
      break;
    case '\"':
      printf("\"");
      break;
    case '\'':
      printf("\'");
      break;
    }
  }
  else
    printf("%c",c);

    return 0;
}

int exec_capet_str_val(char *s);

int exec_capet_str(struct band_list_el *bll){

  char *ptr;

  ptr = (char *)pop_from_stek(&adrstk);
  exec_capet_str_val(ptr);

  return 0;
}


int exec_capet_str_val(char *s){
  
  int i,j,k,pos;
  char *tmp, *ptr, *str;
  
  tmp = (char *)malloc(5); /* ele bele  */
  
  str = s;
  ptr = str;
  j=0;
  pos = 0;
  k = strlen(ptr);
  free(tmp);
  tmp = (char *)malloc(k+1);
  memset(tmp, '\0', k+1);

  while(j<k){
    pos = j;
    while((j<k)&&(*(ptr+j)!='\\')) 
	 j++;
	//if ((j - pos) <= (ptr + pos))
	//	return 0;
    strncpy(tmp, ptr+pos, j-pos);
    *(tmp+j-pos) = '\0';
    printf("%s",tmp);
	 
    if (*(ptr+j)=='\\'){
      char a,b;
      a = *(ptr+j+1);
      switch(a){
      case 'y':
	printf("\n");
	j+=2;
	break;
      case 't':
	printf("\t");
	j+=2;
	break;
      case '\\':
	printf("\\");
	j+=2;
	break;
      case '\"':
	printf("\"");
	j+=2;
	break;
      case '\'':
	printf("\'");
	j+=2;
	break;
      default:
	printf("\\");
	j++;
      }
    }
 
  }

    return 0;
}

void flush_input_bufer(){

  memset(ib, '\0', MAX_INB_LEN);
  ibp = ib;

}

char *get_next_token_from_bufer(){

  char *tbuf, *tbufp, c;
  int st = 0;

  tbuf = (char *)malloc(MAX_INB_LEN);
  memset(tbuf, '\0', MAX_INB_LEN);
  tbufp = tbuf;

  up:

  if (ibp >= ib + MAX_INB_LEN)
    goto out;

    c = *ibp++;

    switch(c){/* discard spaces  */
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      if (st == 1)
	goto out;
      goto up;
      break;
    case '\0':      
      goto out;
    default:
      st = 1;
      *tbufp++ = c;
      goto up;
    }
    
 out:

    if (tbufp != tbuf)
      return tbuf;
    
    return NULL; /* input bufer is empty,flush bufer, read from user*/
}


char *read_from_bufer(){

  char *vstr;

 up:

  vstr = get_next_token_from_bufer();

  if (vstr != NULL)
    return vstr;

  /* bufer is empty, read from user  */
  flush_input_bufer();
  if (fgets(ib, MAX_INB_LEN, stdin)==NULL)
    return NULL; /* cannot read from user  */

  goto up;
  /*TODO: we may hang here, possible loop check is needed in future  */
}



int exec_daxilet_idt(struct band_list_el *bll){

  unsigned char *address;
  char *vstr;
  int tip;

  union val{
    char c;
    long int li;
    long double ld;
  } v;

  vstr = read_from_bufer();

  if (vstr == NULL){
	  printf("Icra xetasi: girish buferinden oxuna bilmir.\n");
    return 0;
  }

  tip = bll->tip;
  address = (unsigned char *)pop_from_stek(&adrstk);

   switch(tip){
    case 1:
      v.li = atoi(vstr);
      memcpy(address, &v, sizeof(long int));
    break;
   case 2:
     v.ld = atof(vstr);
     memcpy(address, &v, sizeof(long double));
    break;
   case 3:
     v.c = *vstr;
   memcpy(address, &v, sizeof(char));
   }

   return 0;
}



int exec_eger_exp(struct band_list_el *bll){

  long double val, *ptr;
  int ival;
  
  ptr = (long double *)pop_from_stek(&valstk);
  val  = *ptr;
  free(ptr); /* always free pointer poped from valstek*/
  ival = (int)val;

  if (ival == 1)
    return 0;
  else
    return bll->head_pos;
}


/* bll-id == YOXSA  */
int exec_yoxsa(struct band_list_el *bll){

  return bll->head_pos;

}




long double get_value(unsigned char *adrs, int tip, int size){

    union val{
    char c;
    long int li;
    long double ld;
  } v;

    switch (tip){
    case 1:
      memcpy(&v.li, adrs, size);
      return (long double)v.li;
    case 2:
      memcpy(&v.ld, adrs, size);
      return (long double)v.ld;
    case 3:    
      memcpy(&v.c, adrs, size);
      return (long double)v.c;
    }

    return -1; /* bug  */
}


/* 
   FUNK in band only to label start of funk kod
   used by init_funcs_ret_adrs()
 */

int exec_func( struct band_list_el *bll){

    return 0;
}



/* bll->id == FCALL1  
   till now pbll was pointing to parent func,
   from now it will point to new one   
 */
int exec_fcall1( struct band_list_el *bll, unsigned char *area){

    free(bll->dt_area);
    bll->dt_area = (unsigned char *)malloc(bll->dt_size);

    /* next code will update adrs space */
    push_to_stek(&fnstk, bll->dt_area);

    /* if called from object update object's adrs space as well */
    if (area != NULL)
      push_to_stek(&obstk, area);      

    return bll->head_pos;
}


/* bll->band_id == PUT_RET_ADDR  
   put return addres to stek
   ret addr in bll->head_pos
 */
int exec_put_ret_addr( struct band_list_el *bll){

	int val, *ptr;

	val = bll->head_pos;
	ptr = (int *)malloc(sizeof(int));
	*ptr = val;
    push_to_stek(&glbstk, (void *)(ptr));

    return 0;
}



/* bll->id == SNMEM  
   get FCALL1 from SNMEM
   push adrs of FCALLL1's parent to obstk
 */
int exec_snmem( struct band_list_el *bll){

  unsigned char *area;

  area = (unsigned char *)pop_from_stek(&adrstk);

  return exec_fcall1(bll, area);
  
}

/* POP from OBSTK which was pushed durion SNMEM call 
   we just need put obstk init state no need adrs from obstk  */
int exec_pop_obstk( struct band_list_el *bll){

  pop_from_stek(&obstk);

  return 0;
}


int exec_neqederki_exp(struct band_list_el *bll){

  return exec_eger_exp(bll);
}


/* bll-id == NEQ_TOP  */
int exec_neq_top(struct band_list_el *bll){

  return bll->head_pos;

}

/* swt var pushed to adrstk and case's  value to valstk
   just compare. 
   !!! Important. DO NOT pop case's variable from adrstk.
   This will be done bu SEC1 after all cases finish their probing. */
int exec_hal1(struct band_list_el *bll){

  int sec_val, hal_val;
  unsigned char *adrs;
  long double *ptr;

  adrs = (unsigned char *)(adrstk.top->dptr);
 /* only variables of types 1 and 3 supported as case and swt values.
    as we keep only swt variables address in adrstk there is no way to 
    get its type. Here we only refer both as from type 1 and hope this
    will work.(int and char) */
  sec_val = (int)get_value(adrs, 1, sizeof(long int));

  ptr = (long double *)pop_from_stek(&valstk);
  hal_val = (int)(*ptr);
  free(ptr);
  
  if (hal_val == sec_val)
    return 0;
  
  return bll->head_pos;
}


/* 
  diff btw hal1 and hal1s is next:

  h1:  if true ret 0(exec code) else ret head_pos(probe another cases or exit)
  h1s: if true ret head_pos (exec code)  else ret 0(probe another cases)
   
 */
int exec_hal1s(struct band_list_el *bll){

  int sec_val, hal_val;
  unsigned char *adrs;
  long double *ptr;

  adrs = (unsigned char *)(adrstk.top->dptr);
 /* only ariables of types 1 and 3 supported as case and swt values.
    as we keep only swt variables address in adrstk there is no way to 
    get its type. Here we only refer both as from type 1 and hope this
    will work.(int and char) */
  /* size of type 1 (tam) defined as long int, nned to change to some makros  */
  sec_val = (int)get_value(adrs, 1, sizeof(long int));
   

  ptr = (long double *)pop_from_stek(&valstk);
  hal_val = (int)(*ptr);
  free(ptr);
  
  if (hal_val != sec_val)
    return 0;
  
  return bll->head_pos;
}

int exec_sus1(struct band_list_el *bll){

  return 0;
}

int exec_free_sec_dyn_stk(struct band_list_el *bll){

  return 0;
}

/* just remove swt variable from adrs stek  */
int exec_secim(struct band_list_el *bll){

  pop_from_stek(&adrstk);

  return 0;
}



int exec_fparm_idt_val(struct band_list_el *bll){
  
  unsigned char *adrs;
  long double *ptr, val;
  int tip;

  tip = bll->tip;
  adrs = get_address(bll);

  ptr = (long double *)pop_from_stek(&valstk);
  val = *ptr;
  free(ptr);

  set_val(adrs, val, tip);

  return 0;
}




/* bll-id == QAYTYAR1  */
int exec_qaytar1(struct band_list_el *bll){

  int *ptr, pos;

	ptr =(int*)pop_from_stek(&glbstk);
  /* obyekt stekini de nezere al  */

	pos = *ptr;
	free(ptr);

	return pos;
}



/* bll-id == DAVAMET1  */
int exec_davamet1(struct band_list_el *bll){

    return bll->head_pos;
}


/* bll-id == DAYAN1  */
int exec_dayan1(struct band_list_el *bll){

  return bll->head_pos;

}

int exec_check_dovr(struct band_list_el *bll){

  unsigned char *adrs;
  long double val;
  void *ptr;
  int tip, size, artsin, sayqac, dovr_son;

  tip = bll->tip;
  size = bll->size;

  ptr = pop_from_stek(&adrstk);
  adrs = (unsigned char *)ptr;
  val = get_value(adrs, tip, size);
  sayqac = (int)val;
  /* !!! do not free ptr popped from adrstk   */ 

  ptr = pop_from_stek(&valstk);
  val = *(long double *)ptr;
  artsin = (int)val;

  ptr = pop_from_stek(&valstk);
  val = *(long double *)ptr;
  dovr_son = (int)val;
  free(ptr);

  if (artsin){
    if (sayqac > dovr_son){
      return bll->head_pos;
    }
  }
  else{
    if (sayqac < dovr_son){
      return bll->head_pos;
    }
  }

return 0;
}



int exec_step_dovr(struct band_list_el *bll){

  unsigned char *adrs;
  long double val;
  void *ptr;
  int tip, size, artsin, sayqac;

  tip = bll->tip;
  size = bll->size;
  ptr = pop_from_stek(&adrstk);
  adrs = (unsigned char *)ptr;
  val  = get_value(adrs, tip, size);
  sayqac = (int)val;
  /* !!! do not free ptr popped from adrstk   */ 

  ptr = pop_from_stek(&valstk);
  val = *(long double *)ptr;
  artsin = (int)val;
  free(ptr);

  if (artsin)
    sayqac++;
  else
    sayqac--;

  val = (long double)sayqac;
  set_val(adrs, val, tip);

  return bll->head_pos;
}

int exec_init_dovr(struct band_list_el *bll){

  exec_asgn(bll);

  return 0;
}


int exec_son(struct band_list_el *bll){

	char ch;
	ch = getchar();
  exit(0);

  //we never reah here, just to make compiler happy
  return 1;
}

/* bll and parent bll  */
int pre_exec_new(struct band_list_el *head){

  struct band_list_el *bll = head;

  int id = bll->band_id;

  switch(id){
  case CAPET_NUMB:
    return exec_capet_numb(bll);
  case CAPET_CHR:
    return exec_capet_chr(bll);
  case CAPET_STR:
    return exec_capet_str(bll);
  case CAPET_FLOAT:
    return exec_capet_float(bll);
  case CAPET_IDT:
    return exec_capet_idt(bll);
  case CAPET_CRG1:
    return exec_capet_crg1(bll);
  case CAPET_CRG2:
    return exec_capet_crg2(bll);
  case CAPET_STRMEM:
    return exec_capet_strmem(bll);
  case CAPET_EXPR:
    return exec_capet_expr(bll);
  case INIT_DOVR:
    return exec_init_dovr(bll);
  case CHECK_DOVR:
    return exec_check_dovr(bll);
  case STEP_DOVR:
    return exec_step_dovr(bll);
  case NEQEDERKI2:
    return exec_neqederki_exp(bll);
  case NEQ_TOP:
    return exec_neq_top(bll);
  case SEC1:
    return exec_secim(bll);
  case HAL1:
    return exec_hal1(bll);
  case HAL1S:
    return exec_hal1s(bll);
  case SUS1:
	  return exec_sus1(bll);
  case FREE_SEC_DYN_STK:
	  return exec_free_sec_dyn_stk(bll);
  case EGER_EXP2:
    return exec_eger_exp(bll);
  case YOXSA:
    return exec_yoxsa(bll);
  case ASGN:
    return exec_asgn(bll);
  case NUMB:
    return exec_numb(bll);
  case ARTSIN:
    return exec_artsin(bll);
  case AZALSIN:
    return exec_azalsin(bll);
  case FLOAT:
    return exec_float(bll);
  case SHERT:
    return exec_shert(bll);
  case CHR:
    return exec_chr(bll);
  case EXPR:
    return exec_expr(bll);
  case STR_ADRS:
    return exec_str_adrs(bll);
  case IDT_ADRS:
    return exec_idt_adrs(bll);
  case IDT_VAL:
    return exec_idt_val(bll);
  case IDT_OFST:
    return exec_idt_ofst(bll);
  case CRG1_ADRS:
    return exec_crg1_adrs(bll);
  case CRG1_VAL:
    return exec_crg1_val(bll);
  case CRG1_OFST:
    return exec_crg1_ofst(bll);
  case CRG2_OFST:
    return exec_crg2_ofst(bll);
  case STRMEM_VAL:
    return exec_strmem_val(bll);
  case CRG2_ADRS:
    return exec_crg2_adrs(bll);
  case CRG2_VAL:
    return exec_crg2_val(bll);
  case DAXILET_IDT:
    return exec_daxilet_idt(bll); 
  case DAVAMET1:
    return exec_davamet1(bll);
  case DAYAN1:
    return exec_dayan1(bll);
  case FUNK:
    return exec_func(bll);
  case FCALL1:
    return exec_fcall1(bll, NULL);
  case SNMEM:
    return exec_snmem(bll);
  case PUT_RET_ADDR:
    return exec_put_ret_addr(bll);
  case POP_OBSTK:
    return exec_pop_obstk(bll);
  case FARGS_NUMB:
    return exec_fargs_numb(bll);
  case FPARM_IDT_VAL:
    return exec_fparm_idt_val(bll);
  case QAYTAR1:
    return exec_qaytar1(bll);
  case SON:
    return exec_son(bll);
 }

  return 0;
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


int main (){

  struct func_table_el *main;
  struct band_list_el *head;
  int pos;
  

  /* init main  */
  /*  main = &glbft->funcs[MAIN_FUNC]; */
 
  /* sorry guys, I just ignored ugly parsing work, so things ended with 
     main funcs(program) local variables which are also programs global variables gatehered as members of global object, so we have to give referense to it from main
  */
  /*  main->dt.count = 0; */

  init_stek();

  /* push main func to fnstk  */
  /*  push_to_stek(&fnstk, main);*/  

  


  /* clear input bufer */
  flush_input_bufer();

  /* build execution band  */
  /*build_exec_band(tree->first->next, exec_band, &band_size);  */
  load_bin("prg.esr_bin", exec_band, &band_size);
  //exit(0);
   // print_exec_band(exec_band, band_size);
   // exit(0);

  /* init glb tips area  */
    glbmt = (struct var_table *)malloc(sizeof(struct var_table));
    glbmt->area = (unsigned char *)malloc(exec_band[0].dt_size);
     /* we saved start adrs [0].head_pos during band build  */
    prg_start = exec_band[0].head_pos;
    push_to_stek(&obstk, glbmt->area);

  // printf("returning from execution.\n");
  //     return;

  head = exec_band + prg_start;

 begin:
 loop:
  //  printf("hpos -> %d\t executing \t%d\n", (int)(head - exec_band));
  pos = pre_exec_new(head);

  //  printf("\t %d moved head to \t%d\n", pos);

  if (pos)
    head = exec_band + pos;
  else
    head++;

    /*    head++;*/

    /*    printf("%p\n%p\n%p\n",head, exec_band + band_size, &exec_band[band_size]);*/

    if (head >= exec_band + band_size)
      goto exit; /* end of program, exit  */

    goto loop;

 exit:
    exit(0);

}
