
#include "esrin.h"
#include "sehvler.h"


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

 
#include <stdlib.h>
 
#include <share.h>



/* ACAR sozler  */
struct token lexems[]={
  {"xaricet", CAPET}, /* diqqet:   evvel xaricet acar sozu evezine capet acarsozu ishlenirdi
  			                       sonradan onu xaricet ile vezledik, lakin CAPET makrosun deyishmedik
								   cunki bu zaman kodda cox deyishiklik etmeliydik
					   */
  {"son", SON},
  {"daxilet", DAXIL},
  {"eger", EGER},
  {"yoxsa", YOXSA},
  {"ve", VE},
  {"veya", VEYA},
  {"funksiya",FUNKSIYA},
  {"qaytar",QAYTAR},
  {"neqederki",NEQEDERKI},
  {"davamet",DAVAMET},
  {"dayan",DAYAN},
  {"sec",SEC},
  {"hal",HAL},
  {"susma",SUSMA},
  {"strukt",STRUKT},
  {"sinif",SINIF},
  {"mexsusi",MEXSUSI},
  {"umumi",UMUMI},
  {"qorunan",QORUNAN},
  {"say", SAY},
/* standart funksiyalara uygun acar sozler  */
  {"STANDART", STANDART},
  {"KVADRAT_KOK", KVADRAT_KOK},
  {"STEKE_QOY", STEKE_QOY},
  {"STEKDEN_GOTUR", STEKDEN_GOTUR}

/* !!!!!!!!!!!! DON'T FORGET UPDATE lxmc below !!!!!!!!!!  */
};

int lxmc = 25;

#define MAX_LINES 10000
char * lines[MAX_LINES];

int lnct = 0;

void init_lines( char *name){

	char *p, *q, *text, *end, **s;
	int fd, size,k;
    
  FILE *f = fopen(name, "rb");
  //fseek(f,0,SEEK_END);
  size = 10; //ftell(f);
 // fseek(f,0,SEEK_SET);

  text = (char *)malloc(size);	
  fread(text, size, 1, f);
  fclose(f);

    end = text + strlen(text);

	p = q = text;

	while (p < end && q<end){

	  while((*q!='\n') && q<end) q++;

	  k = (int)(q-p);
	  s = &lines[lnct++];

      *s = (char *)malloc(k+1);
	  memset(*s,0,k+1);
	  strncpy(*s,p,k);
	  q++;
	  p = q;

        }
}

void print_lines(){
 
  int i;

  for (i=0; i<lnct; ++i)
    printf("%s",lines[i]);
  
}

int crtip = 0;

 char *last_col;
 int cur_row = 1;

void upd_crtip(int tid){

  crtip = tid;

}




#define MAX_NEW_LINE  10000

char *text , *end, *output;

 int nls[MAX_NEW_LINE];
 int nct = 0; /* new_line_count */

#define MAX_STRINGS 10000
#define MAX_BLKS 10000


#define MAX_TOKENS 100000

struct token tokens[MAX_TOKENS];
int tks;
int nl;

void quit(char *s){

  printf("%s\n",s);
  exit(0);
}

int cur_tok = 0;
int las_tok;

int next_tok (){

   las_tok = cur_tok;

   if (cur_tok <= tks){
    cur_tok++;
    return tokens[las_tok].id; }
   else
     return -1;

}

int las_tid(){

   return tokens[las_tok].id;

}




/* parse text and get pos and numb of new lines */



void print_tokens(){

  int i;

  printf("Sizin tokenler\n");
  
  for (i=0; i<tks; i++){
	  printf("i = %d, %s\t\t%d\trow %d   col %d\t",i, tokens[i].tok, tokens[i].id, tokens[i].row, tokens[i].col);
    print_tok(tokens[i].id);
    printf("\n");
  }

}


void print_tokens_with_id(struct dec_table *dec_tab){

  int i;

  printf("Sizin tokenler id ile  \n");
  
  for (i=0; i<tks; i++){
    printf("%s\t\tfid %d\t\ttip %d\t\tofst %d \n", tokens[i].tok, tokens[i].fid, tokens[i].tip, tokens[i].ofst );
  }

}

int get_tok_id(char *tok, int id){


  /* first check for the keywords */

  int i;

  for (i=0; i<lxmc; ++i)
    if (strcmp(lexems[i].tok,tok)==0)
      return lexems[i].id;

  if (id == LEXEM)
    return IDT;
  else
    return NOTYET;

  /* check for string */
 

}

void get_new_token(char *p, char *q, int id, int row, int col){

  char *tmp;
  int k;

  if ((p<text)||(q<p)||(q>end)){
    printf("token sehvi setir %d -de\n",nl);
	err_cnt++;
    return ;
  }

  k  = q - p;
  tmp = (char *)malloc(k+1);
  strncpy(tmp, p, k);
  *(tmp+k) = '\0';
  tokens[tks].tok = tmp;
  if (id == LEXEM || id == NOTYET)
    tokens[tks].id = get_tok_id(tmp,id);
  else
    tokens[tks].id = id;
    tokens[tks].prs_id = id;

   tokens[tks].col = col;
   tokens[tks].row = row;

   tokens[tks].d1 = 1;
   tokens[tks].d2 = 1;

  tks++;

  

}

/* return 1 if p is char   */
int chr(char *p){

  char c = *p;

  if (  ((c >= 'a'  &&  c <= 'z') ||  (c >= 'A'  &&  c <= 'Z')) )
    return 1;

  return 0;

}

/* return 1 if p is digit */
int dig(char *p){

  char c = *p;

  if (   (c >= '0'  &&  c <= '9')   )
    return 1;

  return 0;
}

/* p is char or _ i.e. beg of idt */
static int idt(char *p){

  if ( chr(p) || *p == '_')
    return 1;

  return 0;
}

/* 1 ? p is alfanumeric : 0 */
int alfn(char *p){

  char c = *p;

  if (  idt(p) || dig(p) )
    return 1;

  return 0;

}

int wtch(char *p){

  if (*p == ' ' || *p == '\t'  || *p == '\n' || *p == '\r')
    return SPS;

  if (*p == '\"')
    return STR;

  if (*p == '\'')
    return CHR;

  if (idt(p))
    return IDT;

  if (dig(p))
    return DIG;

  if ((*p == '/') && (*(p+1)=='/'))
    return SLC;

  if ((*p == '/') && (*(p+1)=='*'))
    return MLC;

  if (*p == '+'){

	  if (*(p+1) == '+')
		  return INCREMENT;

	  if (dig(p+1)){
		if ( ((p-1) == text) || (!alfn(p-1) && !(*(p-1) == ')')))
			return DIG;
	  }

	  return POSSIG;
  }

  if (*p == '-'){

	  if (*(p+1) == '-')
		  return DECREMENT;

	  if (dig(p+1)){
		if ( ((p-1) == text) || (!alfn(p-1) && !(*(p-1) == ')')))
			return DIG;
	  }

	  return NEGSIG;
  }
  

  if (*p == ':')
    return COLON;

  if (*p == '[')
    return ARYOPN;

  if (*p == ']')
    return ARYCLS;


  if (*p == '=')
    return ASGN;

  if (*p == '*')
    return MULT;

  if (*p == '/')
    return DEL;

  if (*p == '%')
    return PRCT;


  if (*p == '!')
    return NIDA;


  if (*p == '<')
    return LES;


  if (*p == '>')
    return GRT;

  if (*p == '{')
    return OPNBLK;

  if (*p == '}')
    return CLSBLK;

  if (*p == '.')
    return DOT;

  if (*p == ',')
    return VERGUL;

  if (*p == ';')
    return NOQTEV;

  if (*p == '(')
    return OPNBRK;

  if (*p == ')')
    return CLSBRK;

  return NOTYET;
}


char *fin_spc(char *p){

  while ( p < end && ( *p==' ' ||  *p=='\t' || *p == '\n' || *p == '\r' )){
   // if (*p == '\r' && *(p+1) == '\n'){  changes for windows 7 or xp
	   if (*p == '\r' || *(p) == '\n'){//server 2003
      cur_row++;
    last_col = p;
	p++;
    }
    p++;
  }

  return p;

}



char *fin_str(char *p){

  p++;

  while (p<end && *p!='"'  && *p!= '\n')
    p++;

  return p;
}


char *fin_chr(char *p){

  p++;

  while (p<end && *p!='\''  && *p!= '\n')
    p++;

  return p+1;
}

char *fin_idt(char *p){

  while (p<end && alfn(p))
    p++;

  return p;
}


char *fin_numb(char *p){

  while (p<end && dig(p))
    p++;

  return p;
}

/*  sinlge line comment     */
char *fin_slc(char *p){

  while (p<end && *p != '\n')
    p++;
  
  return p;
}

/*  multy line comment     */
char *fin_mlc(char *p){

  while (p<end && !( *p=='*' &&  *(p+1)=='/')){
    if (*p == '\n' || *p == '\r'){
      cur_row++;
    last_col = p;
    }
	 p++;
  }

  return p+2;

}



/* Deterministic Finit State Automata for recognising 
   single characters  */
void DFA_CHR (char **q){

  char *tmp, *p;
  int state = 0;

  p = *q;
  tmp = p + 1;

 begin:
  
  if (p >= end )
    return;

  p++;
  
  switch(state){
  case 0:

    if (*p == '\''){
      p++;
      *q = p;
      print_token_type_1_err(cur_row, p - last_col,  "\'\'  tek dirnaq arasinda hecne yoxdur");
      return;
    }

    if (*p != '\n' && *p != '\t' && *p != '\r' && *p != '\\' && *p != '\"' ){
      state = 2;
      goto begin;
    }

    if ( *p == '\n' || *p == '\r'){
      p++;
      *q = p;
	  print_token_type_1_err(cur_row, p - last_col,  "\'  tek dirnaq baglayan dirnaq qoyulmayib");
      cur_row++;
      return ;
    }
    
    if ( *p == '\\'){
      state = 8;
      goto begin;
    }

    break;
    
  case 2:
    
    if ( *p == '\n' || p >= end ){
      *q = p;

	  print_token_type_1_err(cur_row, p - last_col,  "\'  tek dirnaq baglayan dirnaq qoyulmayib");
      return ;
    }

    if ( *p == '\''){
      get_new_token(p-1, p, CHR, cur_row, p - last_col);
      p++;
      *q = p;
      return ;
    }

    state = 3;
    goto begin;

  case 3:

    if ( *p == '\n' || p>= end){
	  print_token_type_1_err(cur_row, p - last_col,  "\'  tek dirnaq baglayan dirnaq qoyulmayib");
      *q = p;
      return ;      
    }

    if ( *p == '\''){
      get_new_token(tmp, tmp + 1, CHR, cur_row, tmp - last_col);
      *q = p + 1;
      printf("(%d:%d)xeberdarliq: tek dirnaq icinde bir nece simvol. \n", cur_row, tmp - last_col);
      printf("Ilk simvola qeder qisaldilir\n");
      return ;
    }


    goto begin;

    /* during tests I got that it needed to set both chars to corresp special char
       as if user typed \y 2 chars here \ and y make one spec char '\n' so we need to
       set both \ and y to '\n'. this is how we do it by setting bot *(p-1) and *p
       hope thats clear.  */

  case 8:

     switch(*p){


     case 'y':
       *(p-1) = '\n';
       *p = '\n';
       state = 2;
       goto begin;

     case 'n':
       *(p-1) = '\n';
       *p = '\n';
       state = 2;
       goto begin;

     case 't':
       *(p-1) = '\t';
       *p = '\t';
       state = 2;
       goto begin;

     case '\\':
       *(p-1) = '\\';
       *p = '\\';
       state = 2;
       goto begin;

     case '\'':
       *(p-1) = '\'';
       *p = '\'';
       state = 2;
       goto begin;

     case '\"':
       *(p-1) = '\"';
       *p = '\"';
       state = 2;
       goto begin;
       /*
     case '0':
       *(p-1) = '\0';
       state = 2;
       goto begin;
       */
     }
       

    if ( *p == '\n' || p>= end){
	  print_token_type_1_err(cur_row, p - last_col,  "\'  tek dirnaq baglayan dirnaq qoyulmayib");
      *q = p;
      return ;     
    }

	  print_token_type_2_err(cur_row, p - last_col,  "\\%c  yanlish xususi simvol", *p);
      p++;
      *q = p;
      return;

  }
  

}


 






/* Deterministic Finit State Automata for recognising 
   single NUMBERS  */
void DFA_NUMB (char **q){

  char *tmp, *p, s[100];
  int k, state = 0;

  p = *q;
  tmp = p;

 begin:

  p++;

  switch(state){

  case 0:

    if (dig(p)) break;

    if ( !idt(p) && (*p != '.') ){
      get_new_token(tmp,p, NUMB, cur_row, p - last_col);
      *q = p;
      return;
    }

    if (*p == '.'){
      state = 2; break;
    }

    state = 5;
	break;

  case 2:
    
    if (dig(p)){
      state = 3;
      break;
    }

    state = 5;
    break;

  case 3:
    
    if (dig(p))
      break;

    if (!idt(p) && (*p != '.')){
      get_new_token(tmp,p, FLOAT, cur_row, p - last_col);
      *q = p;
      return;
    }

   state = 5;
   break;

  case 5:
    if (!( idt(p) || dig(p) || *p == '.' ))
      goto error;
  }

  goto begin;
  

 error:
  
  k = p - tmp;
  memset(s, '\0', 100);
  strncpy(s,tmp,k);
  print_token_type_3_err(cur_row, p - last_col,  " duzgun eded deyil ", s);
  *q = p;
  return;


}


int get_file_size(char *name){


char buffer[60000];


   int fh;
   unsigned int nbytes = 60000, bytesread;

   bytesread = 0;

   /* Open file for input: */
   if( _sopen_s( &fh, name, _O_RDONLY, _SH_DENYNO, 0 ) )
   {
      perror( "open failed on input file" );
      exit( 1 );
   }

   /* Read in input: */
   if( ( bytesread = _read( fh, buffer, nbytes ) ) <= 0 )
      perror( "Problem reading file" );
   

   _close( fh );

   return bytesread;   

}

void get_tokens(char *name){
  char *p, *q, tmp[1024];
  int fd,k, size, size_std, nmb;

  /* open src file  */
  FILE *f = fopen(name, "rb");
  fseek(f,0,SEEK_END);
  size = ftell(f);
  fseek(f,0,SEEK_SET);

  /* open std file  */
  FILE *f_std = fopen("standart.esr", "rb");
  fseek(f_std,0,SEEK_END);
  size_std = ftell(f_std);
  fseek(f_std,0,SEEK_SET);

  text = (char *)malloc(size + size_std + 2);
  memset(text, '\0', size + size_std + 2);

  fread(text, size, 1, f);
  fclose(f);

  /* menbe ile standart fayl arasina bir setir artiririq  */
  end = text + size;
  *(end++) = '\n';
  

  /* standart fayli da elave et  */
  fread(end, size_std, 1, f_std);
  fclose(f_std);

  /*printf("%s",text);
  exit(0);
  */

  end = text + size + size_std + 1;

  p = text;
  last_col = text;

  tokens[0].id = FILEBEG;
  tks++;


 while (p<end){
    
    k = wtch(p);

    switch(k){
 
      /* ===================   START of STRING recognition  ====================   */
     case SPS:
       p = fin_spc(p);
       break;
    case STR:
      q = fin_str(p);
      if (q>=end){
	get_new_token(p,q,BADSTR, cur_row, p - last_col);
	print_token_type_1_err(cur_row, p - last_col, " setir xetasi " );
	goto out;
      }
      if (*q == '\n' || *q == '\r'){
	get_new_token(p,q,BADSTR, cur_row, p - last_col);
	print_token_type_1_err(cur_row, p - last_col, " setir xetasi " );
	p = q+1;
    cur_row++;
	last_col = p;
	break;
      }
      /* found string q pointing to last quote  
	 update q to point next char   */
      q++;      
      get_new_token(p+1,q-1, STR, cur_row, p - last_col);
      p = q;
      break;
      /* ===================   END of STRING recognition  ====================     */

    case CHR:
      // Let DFA do the job
      DFA_CHR(&p);
      break;       
 
    case IDT:
      q = fin_idt(p);
      get_new_token(p,q, LEXEM, cur_row, p - last_col);
      p = q;
      break;

    case DIG:
      // Let DFA do the job
      DFA_NUMB(&p);
      break;

    case SLC:
      p = fin_slc(p);
      break;
    case MLC:
      p = fin_mlc(p);
      break;

    case POSSIG:
      get_new_token(p,p+1, POSSIG, cur_row, p - last_col);  
      p++;
      break;

    case NEGSIG:
      get_new_token(p,p+1, NEGSIG, cur_row, p - last_col);   
      p++;
      break;

	case DECREMENT:
      get_new_token(p,p+2, DECREMENT, cur_row, p - last_col);   
      p += 2;
      break;

	case INCREMENT:
      get_new_token(p,p+2, INCREMENT, cur_row, p - last_col);   
      p += 2;
      break;

    case COLON:
      get_new_token(p,p+1, COLON, cur_row, p - last_col);   
      p++;
      break;

    case ARYOPN:
      get_new_token(p,p+1, ARYOPN, cur_row, p - last_col);   
      p++;
      break;

    case ARYCLS:
      get_new_token(p,p+1, ARYCLS, cur_row, p - last_col);   
      p++;
      break;

    case ASGN:
      if (*(p+1)=='='){
      get_new_token(p,p+2, BERABER, cur_row, p - last_col);   
      p+=2;}
      else{
      get_new_token(p,p+1, ASGN, cur_row, p - last_col);   
      p++;}
      break;

    case MULT:
      get_new_token(p,p+1, MULT, cur_row, p - last_col);   
      p++;
      break;

    case DEL:
      get_new_token(p,p+1, DEL, cur_row, p - last_col);   
      p++;
      break;

    case PRCT:
      get_new_token(p,p+1, PRCT, cur_row, p - last_col);   
      p++;
      break;

    case NIDA:
      if (*(p+1)=='='){
      get_new_token(p,p+2, FERQLI, cur_row, p - last_col);   
      p+=2;}
      else{
      get_new_token(p,p+1, NIDA, cur_row, p - last_col);   
      p++;      
      }

      break;


    case LES:
      if (*(p+1)=='='){
      get_new_token(p,p+2, LESEQU, cur_row, p - last_col);   
      p+=2;
      }
      else{
      get_new_token(p,p+1, LES, cur_row, p - last_col);   
      p++;}
      break;


    case GRT:
      if (*(p+1)=='='){
      get_new_token(p,p+2, GRTEQU, cur_row, p - last_col);   
      p+=2;
      }
      else{
      get_new_token(p,p+1, GRT, cur_row, p - last_col);   
      p++;
      }
      break;


    case OPNBLK:
      get_new_token(p,p+1, OPNBLK, cur_row, p - last_col);   
      p++;
      break;


    case CLSBLK:
      get_new_token(p,p+1, CLSBLK, cur_row, p - last_col);   
      p++;
      break;


    case DOT:
      get_new_token(p,p+1, DOT, cur_row, p - last_col);   
      p++;
      break;


    case VERGUL:
      get_new_token(p,p+1, VERGUL, cur_row, p - last_col);   
      p++;
      break;


    case NOQTEV:
      get_new_token(p,p+1, NOQTEV , cur_row, p - last_col);   
      p++;
      break;


    case OPNBRK:
      get_new_token(p,p+1, OPNBRK, cur_row, p - last_col);   
      p++;
      break;


    case CLSBRK:
      get_new_token(p,p+1, CLSBRK, cur_row, p - last_col);   
      p++;
      break;


     default:
      get_new_token(p,p+1, NOTYET, cur_row, p - last_col);
      p++;
    }

}

  out:

  /* ola bilsin gbu yaxshi qayda deyil, amma sehvleri yoxlayan kod 
     bir nece token teleb etdiyinden, bu bosh tokenleri elave edirik,
     hec olmasa bashlangicda sehvi deye bilmek ucun  */
  tokens[tks].id = FILESON;
  tks++;

  tokens[tks].id = FILESON;
  tks++;

  tokens[tks].id = FILESON;
  tks++;

  tokens[tks].id = FILESON;
  tks++;

  tokens[tks].id = FILESON;
  tks++;

  *p = 1;

}

 


void hash(unsigned char *str, unsigned long *h){

  unsigned long hash = 5381;

  int c;

  if (str == NULL){
    *h =  0;
    return;
  }

  while (c = *str){
    hash = ((hash<<5)+hash)+c;
    str++;
  }

  *h = hash;
}


void init_hash_ids(){

  unsigned long h;
  int i;

  for (i=0; i<tks; ++i){
    hash((unsigned char *)tokens[i].tok, &h);
    tokens[i].hid = h;
  }

}

void print_hash(){

  int i;

  for (i=0; i<tks; ++i)
    printf("%s\t%lu\n",tokens[i].tok, tokens[i].hid);
}


char *get_name (unsigned long hid){

  int i;

  for (i = 0; i<tks; ++i){
    if (tokens[i].hid == hid)
      return tokens[i].tok;
  }

    return NULL;
}



void print_tok(int tok){

  switch(tok){

  case  PRG:
    printf("PRG");
    break;
  case  VAR:
    printf("VAR");
    break;
  case  TAM:
    printf("TAM");
    break;
  case  KESR:
    printf("KESR");
    break;
  case  SIMVOL:
    printf("SIMVOL");
    break;
  case  IDT:
    printf("IDT");
    break;
  case  VERGUL:
    printf("VERGUL");
    break;
  case  NOQTEV:
    printf("NOQTEV");
    break;
  case  KOD:
    printf("KOD");
    break;
  case  CAPET:
    printf("CAPET");
    break;
  case  OPNBRK:
    printf("OPNBRK");
    break;
  case  STR:
    printf("STR");
    break;
  case  CHR:
    printf("CHR");
    break;
  case  CLSBRK:
    printf("CLSBRK");
    break;
  case  SON:
    printf("SON");
    break;
  case  DOT:
    printf("DOT");
    break;
  case  LEXEM:
    printf("LEX");
    break;
  case  NUMB:
    printf("NUMB");
    break;
  case  SIGN:
    printf("SIGN");
    break;
  case  DIG:
    printf("DIG");
    break;
  case  POSDIG:
    printf("POSDIG" );
    break;
  case  POSNUMB:
    printf("POSNUMB");
    break;
  case  SNUMB:
    printf("SNUMB");
    break;
  case  POSSIG:
    printf("POSSIG");
    break;
  case  NEGSIG:
    printf("NEGSIG");
    break;
  case  SLC:
    printf("SLC");
    break;
  case  MLC:
    printf("MLC");
    break;
  case  SPS:
    printf("SPS");
    break;
  case  COLON:
    printf("COLON");
    break;
  case  ARYOPN:
    printf("ARYOPN");
    break;
  case  ARYCLS:
    printf("ARYCLS");
    break;
  case  ASGN:
    printf("ASGN");
    break;
  case  MULT:
    printf("MULT");
    break;
  case  DEL:
    printf("DEL");
    break;
  case  PRCT:
    printf("PRCT");
    break;
  case  XAR:
    printf("XAR");
    break;
  case  DAXIL:
    printf("DAXIL");
    break;
  case  EGER:
    printf("EGER");
    break;
  case  NIDA:
    printf("NIDA");
    break;
  case  GRT:
    printf("GRT");
    break;
  case  LES:
    printf("LES");
    break;
  case  VE:
    printf("VE");
    break;
  case  VEYA:
    printf("VEYA");
    break;
  case  FILESON:
    printf("FILESON");
    break;
  case  OPNBLK:
    printf("OPNBLK");
    break;
  case  CLSBLK:
    printf("CLSBLK");
    break;
  case  ONDA:
    printf("ONDA");
    break;
  case  YOXSA:
    printf("YOXSA");
    break;
  case  BERABER:
    printf("BERABER");
    break;
  case  FERQLI:
    printf("FERQLI");
    break;
  case  LESEQU:
    printf("LESEQU");
    break;
  case  GRTEQU:
    printf("GRTEQU");
    break;
  case  EMPTOK:
    printf("EMPTOK");
    break;
  case  SHERT:
    printf("SHERT");
    break;
  case  IFAD:
    printf("IFAD");
    break;
  case  ARTSIN:
    printf("ARTSIN");
    break;
  case  AZALSIN:
    printf("AZALSIN");
    break;
  case  ADDIM:
    printf("ADDIM");
    break;
  case  DOVR:
    printf("DOVR");
    break;
  case  KIMI:
    printf("KIMI");
    break;
  case  CRG1:
    printf("CRG1");
    break;
  case  CRG2:
    printf("CRG2");
    break;
  case  TEKRARET:
    printf("TEKRARET");
    break;
  case  DAN:
    printf("DAN");
    break;
  case  DEN:
    printf("DEN");
    break;

  case  CAPET_OP:
    printf("CAPET_OP");
    break;
  case  DOVR_OP:
    printf("DOVR_OP");
    break;
  case  BLOCK:
    printf("BLOCK");
    break;
  case  BIRTIP:
    printf("BIRTIP");
    break;
  case  TIPLER:
    printf("TIPLER");
    break;
  case  EGER_OP:
    printf("EGER_OP");
    break;
  case  ASGN_OP:
    printf("ASGN_OP");
    break;
  case  DAXIL_OP:
    printf("DAXIL_OP");
    break;
  case  BLOKSUZ:
    printf("BLOKSUZ");
    break;
  case  BLOKLU:
    printf("BLOKLU");
    break;
  case  KODDATALAR:
    printf("KODDATALAR");
    break;


  case  BLOKLUDATA:
    printf("BLOKLUDATA");
    break;

  case  DOVR_EXP:
    printf("DOVR_EXP");
    break;
  case  EGER_EXP:
    printf("EGER_EXP");
    break;

  case  DOVR_BLOKSUZ:
    printf("DOVR_BLOKSUZ");
    break;
  case  EGER_BLOKSUZ:
    printf("EGER_BLOKSUZ");
    break;
  case  YOXSA_BLOKSUZ:
    printf("YOXSA_BLOKSUZ");
    break;

  case  KODDATA:
    printf("KODDATA");
    break;

  case  DOVR_BLOKLU:
    printf("DOVR_BLOKLU");
    break;
  case  EGER_BLOKLU:
    printf("EGER_BLOKLU");
    break;
  case  YOXSA_BLOKLU:
    printf("YOXSA_BLOKLU");
    break;
  case  EGER_TEK_BLOKSUZ:
    printf("EGER_TEK_BLOKSUZ");
    break;
  case  EGER_TAM_BLOKSUZ:
    printf("EGER_TAM_BLOKSUZ");
    break;

  case  TAM1:
    printf("TAM1");
    break;
  case  TAM2:
    printf("TAM2");
    break;
  case  KESR1:
    printf("KESR1");
    break;
  case  KESR2:
    printf("KESR2");
    break;
  case  SIMVOL1:
    printf("SIMVOL1");
    break;
  case  SIMVOL2:
    printf("SIMVOL2");
    break;
 case  VAR1:
    printf("VAR1");
    break;
  case  VAR2:
    printf("VAR2");
    break;

 case  DAXILET1:
    printf("DAXILET1");
    break;
  case  DAXILET2:
    printf("DAXILET2");
    break;
 case  CAPET1:
    printf("CAPET1");
    break;
  case  CAPET2:
    printf("CAPET2");
    break;

 case  EGER_EXP1:
    printf("EGER_EXP1");
    break;
  case  EGER_EXP2:
    printf("EGER_EXP2");
    break;

  case  KOD1:
    printf("KOD1");
    break;

  case  PRG1:
    printf("PRG1");
    break;


  case  BADSTR:
    printf("BADSTR");
    break;

 case  BADCHR:
    printf("BADCHR");
    break;

  case  BADIDT:
    printf("BADIDT");
    break;

  case  DAXILET3:
    printf("DAXILET3");
    break;

  case  EMPTYOP:
    printf("EMTYOP");
    break;

  case  CAPET3:
    printf("CAPET3");
    break;

  case  EXPR:
    printf("EXPR");
    break;

  case  BRK:
    printf("BRK");
    break;


 case  FILEBEG:
    printf("FILEBEG");
    break;

 case  FLOAT_HALF:
    printf("FLOAT_HALF");
    break;


 case  FLOAT:
    printf("FLOAT");
    break;

 case  FUNKSIYA:
    printf("FUNKSIYA");
    break;


 case  FUNK_ELAN:
    printf("FUNK_ELAN");
    break;


 case  FUNK_BODY:
    printf("FUNK_BODY");
    break;


 case  EMPBRK:
    printf("EMPBRK");
    break;

 case  FE1:
    printf("FE1");
    break;

 case  FE2:
    printf("FE2");
    break;

 case  FUNK:
    printf("FUNK");
    break;

 case  FUNKS:
    printf("FUNKS");
    break;

 case  FCALL:
    printf("FCALL");
    break;

 case  FCALL1:
    printf("FCALL1");
    break;

 case  VDEC:
    printf("VDEC");
    break;


 case  DUMMY:
   printf(" ");  /* don't print dummy tokens, they play no role during parsing */
    break;

  case  FDECL:
    printf("FDECL");
    break;
	
  case  PRG2:
    printf("PRG2");
    break;

  case  QAYTAR:
    printf("QAYTAR");
    break;

 case  QAYTAR1:
    printf("QAYTAR1");
    break;

 case  NEQEDERKI:
    printf("NEQEDERKI");
    break;

 case  NEQEDERKI1:
    printf("NEQEDERKI1");
    break;

 case  NEQEDERKI2:
    printf("NEQEDERKI2");
    break;

 case  DAVAMET:
    printf("DAVAMET");
    break;

 case  DAYAN:
    printf("DAYAN");
    break;

 case  DAVAMET1:
    printf("DAVAMET1");
    break;

 case  DAYAN1:
    printf("DAYAN1");
    break;

 case  SEC:
    printf("SEC");
    break;

 case  SEC_OP:
    printf("SEC_OP");
    break;

 case  HAL:
    printf("HAL");
    break;

 case  HAL1:
    printf("HAL1");
    break;

 case  HAL2:
    printf("HAL2");
    break;

 case  HAL3:
    printf("HAL3");
    break;

 case  SEC1:
    printf("SEC1");
    break;

 case  SUSMA:
    printf("SUSMA");
    break;

case  START:
    printf("S");
    break;

case  STRUKT:
    printf("STRUKT");
    break;

case  STRUKT1:
    printf("STRUKT1");
    break;

case  STRUKT2:
    printf("STRUKT2");
    break;

case  STRUKT3:
    printf("STRUKT3");
    break;

case  YNT1:
    printf("YNT1");
    break;

case  YNT2:
    printf("YNT2");
    break;

case  STRMEM:
    printf("STRMEM");
    break;


case  YNTS:
    printf("YNTS");
    break;

case  SINIF:
    printf("SINIF");
    break;

case  QORUNAN:
    printf("QORUNAN");
    break;

case  UMUMI:
    printf("UMUMI");
    break;

case  MEXSUSI:
    printf("MEXSUSI");
    break;

case  SINIF_HEAD:
    printf("SINIF_HEAD");
    break;

case  SINIF_HEAD_START:
    printf("SINIF_HEAD_START");
    break;

case  SINIF_BODY:
    printf("SINIF_BODY");
    break;

case  SINIF_DECL:
    printf("SINIF_DECL");
    break;

case  SINIF_TYPE:
    printf("SINIF_TYPE");
    break;

case  YENI_TIP:
    printf("YENI_TIP");
    break;

case  SNMEM:
    printf("SNMEM");
    break;



case  INIT_DOVR:
    printf("INIT_DOVR");
    break;

case  CHECK_DOVR:
    printf("CHECK_DOVR");
    break;

case  STEP_DOVR:
    printf("STEP_DOVR");
    break;

case NEQ_TOP:
    printf("NEQ_TOP");
    break;

case SUS1:
    printf("SUS1");
    break;

case HAL1S:
    printf("HAL1S");
    break;

  case FARGS:
    printf("FARGS");
    break;

  case FPARM:
    printf("FPARM");
    break;

  case FCALL_BEG:
    printf("FCALL_BEG");
    break;


  case PUT_RET_ADDR:
    printf("PUT_RET_ADDR");
    break;


  case IDT_ADRS:
    printf("IDT_ADRS");
    break;

  case IDT_VAL:
    printf("IDT_VAL");
    break;

  case CRG1_ADRS:
    printf("CRG1_ARDS");
    break;

  case CRG2_ADRS:
    printf("CRG2_ADRS");
    break;

  case CRG1_VAL:
    printf("CRG1_VAL");
    break;

  case CRG2_VAL:
    printf("CRG2_VAL");
    break;

  case IDT_OFST:
    printf("IDT_OFST");
    break;

  case CRG1_OFST:
    printf("CRG1_OFST");
    break;

  case CRG2_OFST:
    printf("CRG2_OFST");
    break;

  case STRMEM_VAL:
    printf("STRMEM_VAL");
    break;

  case CAPET_NUMB:
    printf("CAPET_NUMB");
    break;
  case CAPET_FLOAT:
    printf("CAPET_FLOAT");
    break;
  case CAPET_CHR:
    printf("CAPET_CHR");
    break;
  case CAPET_STR:
    printf("CAPET_STR");
    break;
  case CAPET_IDT:
    printf("CAPET_IDT");
    break;
  case CAPET_CRG1:
    printf("CAPET_CRG1");
    break;
  case CAPET_CRG2:
    printf("CAPET_CRG2");
    break;
  case CAPET_STRMEM:
    printf("CAPET_STRMEM");
    break;
  case CHR_ADRS:
    printf("CHR_ADRS");
    break;
  case STR_ADRS:
    printf("STR_ADRS");
    break;

  case CAPET_EXPR:
    printf("CAPET_EXPR");
    break;

  case DAXILET_IDT:
    printf("DAXILET_IDT");
    break;

  case FARGS_NUMB:
    printf("FAGRS_NUMB");
    break;

  case FARGS_FLOAT:
    printf("FAGRS_FLOAT");
    break;

  case FARGS_CHR:
    printf("FAGRS_CHR");
    break;

  case FARGS_STR:
    printf("FAGRS_STR");
    break;

  case FARGS_IDT_VAL:
    printf("FAGRS_IDT_VAL");
    break;

  case FARGS_CRG1_VAL:
    printf("FAGRS_CRG1_VAL");
    break;

  case FARGS_CRG2_VAL:
    printf("FAGRS_CRG2_VAL");
    break;

  case FARGS_STRMEM_VAL:
    printf("FAGRS_STRMEM_VAL");
    break;

  case FARGS_EXPR_VAL:
    printf("FAGRS_EXPR_VAL");
    break;

  case FPARM_IDT_VAL:
    printf("FPARM_IDT_VAL");
    break;

  case POP_OBSTK:
    printf("POP_OBSTK");
    break;
    

case GLBMT:
    printf("GLBMT");
    break;

case FREE_SEC_DYN_STK:
	printf("FREE_SEC_DYN_STK");
	break;

case KVK:
	printf("KVK");
	break;

case KVK1:
	printf("KVK1");
	break;

case POP_FNSTK:
    printf("POP_FNSTK");
    break;

case GET_FUNC_RET_RES:
    printf("GET_FUNC_RET_RES");
    break;

case PUT_FUNC_RET_RES:
    printf("PUT_FUNC_RET_RES");
    break;

case KOD_WITH_VAR:
    printf("KOD_WITH_VAR");
    break;

case KOD_FUNK:
    printf("KOD_FUNK");
    break;

case TEXT:
    printf("TEXT");
    break;

case STANDART:
    printf("STANDART");
    break;

case STANDART_FUNK:
    printf("STANDART_FUNK");
    break;

case KVADRAT_KOK:
    printf("KVADRAT_KOK");
    break;

case STEKE_QOY:
    printf("STEKE_QOY");
    break;

case STEKE_QOY_STD:
    printf("STEKE_QOY_STD");
    break;

case STEKDEN_GOTUR:
    printf("STEKDEN_GOTUR");
    break;

case STEKDEN_GOTUR_STD:
    printf("STEKDEN_GOTUR_STD");
    break;

case SAY:
	printf("SAY");
	break;

case SAY_TIP_1:
	printf("SAY_TIP_1");
	break;

case CHECK_SAY_TIP_1:
	printf("CHECK_SAY_TIP_1");
	break;

case STEP_SAY_TIP_1:
	printf("STEP_SAY_TIP_1");
	break;

case YNT1_FUNC_ARGS:
	printf("YNT1_FUNC_ARGS");
	break;


case SAY_TIP_2:
	printf("SAY_TIP_2");
	break;

case SAY_TIP_3:
	printf("SAY_TIP_3");
	break;

case CHECK_SAY_TIP_2:
	printf("CHECK_SAY_TIP_2");
	break;

case STEP_SAY_TIP_2:
	printf("STEP_SAY_TIP_2");
	break;

case CHECK_SAY_TIP_3:
	printf("CHECK_SAY_TIP_3");
	break;

case STEP_SAY_TIP_3:
	printf("STEP_SAY_TIP_3");
	break;

case INIT_SAY_TIP_2:
	printf("INIT_SAY_TIP_2");
	break;

case INIT_SAY_TIP_3:
	printf("INIT_SAY_TIP_3");
	break;

case INCREMENT:
	printf("INCREMENT");
	break;

case DECREMENT:
	printf("DECREMENT");
	break;

case INCREMENT_OPR:
	printf("INCREMENT_OPR");
	break;

case DECREMENT_OPR:
	printf("DECREMENT_OPR");
	break;

case DAXIL_INSIDE:
	printf("DAXIL_INSIDE");
	break;

case CAPET_INSIDE:
	printf("CAPET_INSIDE");
	break;

case EGER_INSIDE:
	printf("EGER_INSIDE");
	break;

case ARRAY_INDEX:
	printf("ARRAY_INDEX");
	break;

case FCALL_VDEC:
	printf("FCALL_VDEC");
	break;

case SHERT_RIGHT:
	printf("SHERT_RIGHT");
	break;

case ASGN_RIGHT:
	printf("ASGN_RIGHT");
	break;

case SAY_INSIDE:
	printf("SAY_INSIDE");
	break;

case FUNKSIYA_ELAN:
	printf("FUNKSIYA_ELAN");
	break;

case BLOCK_INSIDE:
	printf("BLOCK_INSIDE");
	break;

case SINIF_INSIDE:
	printf("SINIF_INSIDE");
	break;

case FREE_CONTEXT:
	printf("FREE_CONTEXT");
	break;


case  NOTYET:
    printf("NOTYET");
    break;
  }
}



void print_tok_str(int tok){

  switch(tok){

  case  PRG:
    printf("PRG");
    break;
  case  VAR:
    printf("VAR");
    break;
  case  TAM:
    printf("TAM");
    break;
  case  KESR:
    printf("KESR");
    break;
  case  SIMVOL:
    printf("SIMVOL");
    break;
  case  IDT:
    printf("deyishen");
    break;
  case  VERGUL:
    printf("`,`");
    break;
  case  NOQTEV:
    printf("`;`");
    break;
  case  KOD:
    printf("KOD");
    break;
  case  CAPET:
    printf("CAPET");
    break;
  case  OPNBRK:
    printf("`(`");
    break;
  case  STR:
    printf("setir");
    break;
  case  CHR:
    printf("simvol");
    break;
  case  CLSBRK:
    printf("`)`");
    break;
  case  SON:
    printf("SON");
    break;
  case  DOT:
    printf("`.`");
    break;
  case  LEXEM:
    printf("LEX");
    break;
  case  NUMB:
    printf("eded");
    break;
  case  SIGN:
    printf("ishare");
    break;
  case  DIG:
    printf("reqem");
    break;
  case  POSDIG:
    printf("POSDIG" );
    break;
  case  POSNUMB:
    printf("POSNUMB");
    break;
  case  SNUMB:
    printf("SNUMB");
    break;
  case  POSSIG:
    printf("`+`");
    break;
  case  NEGSIG:
    printf("`-`");
    break;
  case  SLC:
    printf("SLC");
    break;
  case  MLC:
    printf("MLC");
    break;
  case  SPS:
    printf("SPS");
    break;
  case  COLON:
	  printf("`:`");
    break;
  case  ARYOPN:
    printf("`[`");
    break;
  case  ARYCLS:
    printf("`]`");
    break;
  case  ASGN:
    printf("`=`");
    break;
  case  MULT:
    printf("`*`");
    break;
  case  DEL:
    printf("DEL");
    break;
  case  PRCT:
    printf("PRCT");
    break;
  case  XAR:
    printf("XAR");
    break;
  case  DAXIL:
    printf("DAXIL");
    break;
  case  EGER:
    printf("eger");
    break;
  case  NIDA:
    printf("NIDA");
    break;
  case  GRT:
    printf("`>`");
    break;
  case  LES:
    printf("`<`");
    break;
  case  VE:
    printf("ve");
    break;
  case  VEYA:
    printf("veya");
    break;
  case  FILESON:
    printf("FILESON");
    break;
  case  OPNBLK:
	  printf("`{`");
    break;
  case  CLSBLK:
	  printf("`}`");
    break;
  case  ONDA:
    printf("ONDA");
    break;
  case  YOXSA:
    printf("yoxsa");
    break;
  case  BERABER:
    printf("`==`");
    break;
  case  FERQLI:
    printf("`!=`");
    break;
  case  LESEQU:
    printf("LESEQU");
    break;
  case  GRTEQU:
    printf("GRTEQU");
    break;
  case  EMPTOK:
    printf("EMPTOK");
    break;
  case  SHERT:
    printf("shert");
    break;
  case  IFAD:
    printf("IFAD");
    break;
  case  ARTSIN:
    printf("ARTSIN");
    break;
  case  AZALSIN:
    printf("AZALSIN");
    break;
  case  ADDIM:
    printf("ADDIM");
    break;
  case  DOVR:
    printf("DOVR");
    break;
  case  KIMI:
    printf("KIMI");
    break;
  case  CRG1:
    printf("CRG1");
    break;
  case  CRG2:
    printf("CRG2");
    break;
  case  TEKRARET:
    printf("TEKRARET");
    break;
  case  DAN:
    printf("DAN");
    break;
  case  DEN:
    printf("DEN");
    break;

  case  CAPET_OP:
    printf("CAPET_OP");
    break;
  case  DOVR_OP:
    printf("DOVR_OP");
    break;
  case  BLOCK:
    printf("BLOCK");
    break;
  case  BIRTIP:
    printf("BIRTIP");
    break;
  case  TIPLER:
    printf("TIPLER");
    break;
  case  EGER_OP:
    printf("EGER_OP");
    break;
  case  ASGN_OP:
    printf("ASGN_OP");
    break;
  case  DAXIL_OP:
    printf("DAXIL_OP");
    break;
  case  BLOKSUZ:
    printf("BLOKSUZ");
    break;
  case  BLOKLU:
    printf("BLOKLU");
    break;
  case  KODDATALAR:
    printf("KODDATALAR");
    break;


  case  BLOKLUDATA:
    printf("BLOKLUDATA");
    break;

  case  DOVR_EXP:
    printf("DOVR_EXP");
    break;
  case  EGER_EXP:
    printf("EGER_EXP");
    break;

  case  DOVR_BLOKSUZ:
    printf("DOVR_BLOKSUZ");
    break;
  case  EGER_BLOKSUZ:
    printf("EGER_BLOKSUZ");
    break;
  case  YOXSA_BLOKSUZ:
    printf("YOXSA_BLOKSUZ");
    break;

  case  KODDATA:
    printf("emeliyyat");
    break;

  case  DOVR_BLOKLU:
    printf("DOVR_BLOKLU");
    break;
  case  EGER_BLOKLU:
    printf("EGER_BLOKLU");
    break;
  case  YOXSA_BLOKLU:
    printf("YOXSA_BLOKLU");
    break;
  case  EGER_TEK_BLOKSUZ:
    printf("EGER_TEK_BLOKSUZ");
    break;
  case  EGER_TAM_BLOKSUZ:
    printf("EGER_TAM_BLOKSUZ");
    break;

  case  TAM1:
    printf("TAM1");
    break;
  case  TAM2:
    printf("TAM2");
    break;
  case  KESR1:
    printf("KESR1");
    break;
  case  KESR2:
    printf("KESR2");
    break;
  case  SIMVOL1:
    printf("SIMVOL1");
    break;
  case  SIMVOL2:
    printf("SIMVOL2");
    break;
 case  VAR1:
    printf("VAR1");
    break;
  case  VAR2:
    printf("VAR2");
    break;

 case  DAXILET1:
    printf("DAXILET1");
    break;
  case  DAXILET2:
    printf("DAXILET2");
    break;
 case  CAPET1:
    printf("CAPET1");
    break;
  case  CAPET2:
    printf("CAPET2");
    break;

 case  EGER_EXP1:
    printf("EGER_EXP1");
    break;
  case  EGER_EXP2:
    printf("EGER_EXP2");
    break;

  case  KOD1:
    printf("KOD1");
    break;

  case  PRG1:
    printf("PRG1");
    break;


  case  BADSTR:
    printf("BADSTR");
    break;

 case  BADCHR:
    printf("BADCHR");
    break;

  case  BADIDT:
    printf("BADIDT");
    break;

  case  DAXILET3:
    printf("DAXILET3");
    break;

  case  EMPTYOP:
    printf("EMTYOP");
    break;

  case  CAPET3:
    printf("CAPET3");
    break;

  case  EXPR:
    printf("EXPR");
    break;

  case  BRK:
    printf("BRK");
    break;


 case  FILEBEG:
    printf("FILEBEG");
    break;

 case  FLOAT_HALF:
    printf("FLOAT_HALF");
    break;


 case  FLOAT:
    printf("FLOAT");
    break;

 case  FUNKSIYA:
    printf("FUNKSIYA");
    break;


 case  FUNK_ELAN:
    printf("FUNK_ELAN");
    break;


 case  FUNK_BODY:
    printf("FUNK_BODY");
    break;


 case  EMPBRK:
    printf("EMPBRK");
    break;

 case  FE1:
    printf("FE1");
    break;

 case  FE2:
    printf("FE2");
    break;

 case  FUNK:
    printf("FUNK");
    break;

 case  FUNKS:
    printf("FUNKS");
    break;

 case  FCALL:
    printf("FCALL");
    break;

 case  FCALL1:
    printf("FCALL1");
    break;

 case  VDEC:
    printf("VDEC");
    break;


 case  DUMMY:
   printf(" ");  /* don't print dummy tokens, they play no role during parsing */
    break;

  case  FDECL:
    printf("FDECL");
    break;
	
  case  PRG2:
    printf("PRG2");
    break;

  case  QAYTAR:
    printf("QAYTAR");
    break;

 case  QAYTAR1:
    printf("QAYTAR1");
    break;

 case  NEQEDERKI:
    printf("NEQEDERKI");
    break;

 case  NEQEDERKI1:
    printf("NEQEDERKI1");
    break;

 case  NEQEDERKI2:
    printf("NEQEDERKI2");
    break;

 case  DAVAMET:
    printf("DAVAMET");
    break;

 case  DAYAN:
    printf("DAYAN");
    break;

 case  DAVAMET1:
    printf("DAVAMET1");
    break;

 case  DAYAN1:
    printf("DAYAN1");
    break;

 case  SEC:
    printf("SEC");
    break;

 case  SEC_OP:
    printf("SEC_OP");
    break;

 case  HAL:
    printf("HAL");
    break;

 case  HAL1:
    printf("HAL1");
    break;

 case  HAL2:
    printf("HAL2");
    break;

 case  HAL3:
    printf("HAL3");
    break;

 case  SEC1:
    printf("SEC1");
    break;

 case  SUSMA:
    printf("SUSMA");
    break;

case  START:
    printf("S");
    break;

case  STRUKT:
    printf("STRUKT");
    break;

case  STRUKT1:
    printf("STRUKT1");
    break;

case  STRUKT2:
    printf("STRUKT2");
    break;

case  STRUKT3:
    printf("STRUKT3");
    break;

case  YNT1:
    printf("YNT1");
    break;

case  YNT2:
    printf("YNT2");
    break;

case  STRMEM:
    printf("STRMEM");
    break;


case  YNTS:
    printf("YNTS");
    break;

case  SINIF:
    printf("SINIF");
    break;

case  QORUNAN:
    printf("QORUNAN");
    break;

case  UMUMI:
    printf("UMUMI");
    break;

case  MEXSUSI:
    printf("MEXSUSI");
    break;

case  SINIF_HEAD:
    printf("SINIF_HEAD");
    break;

case  SINIF_HEAD_START:
    printf("SINIF_HEAD_START");
    break;

case  SINIF_BODY:
    printf("SINIF_BODY");
    break;

case  SINIF_DECL:
    printf("SINIF_DECL");
    break;

case  SINIF_TYPE:
    printf("SINIF_TYPE");
    break;

case  YENI_TIP:
    printf("YENI_TIP");
    break;

case  SNMEM:
    printf("SNMEM");
    break;



case  INIT_DOVR:
    printf("INIT_DOVR");
    break;

case  CHECK_DOVR:
    printf("CHECK_DOVR");
    break;

case  STEP_DOVR:
    printf("STEP_DOVR");
    break;

case NEQ_TOP:
    printf("NEQ_TOP");
    break;

case SUS1:
    printf("SUS1");
    break;

case HAL1S:
    printf("HAL1S");
    break;

  case FARGS:
    printf("FARGS");
    break;

  case FPARM:
    printf("FPARM");
    break;

  case FCALL_BEG:
    printf("FCALL_BEG");
    break;


  case PUT_RET_ADDR:
    printf("PUT_RET_ADDR");
    break;


  case IDT_ADRS:
    printf("IDT_ADRS");
    break;

  case IDT_VAL:
    printf("IDT_VAL");
    break;

  case CRG1_ADRS:
    printf("CRG1_ARDS");
    break;

  case CRG2_ADRS:
    printf("CRG2_ADRS");
    break;

  case CRG1_VAL:
    printf("CRG1_VAL");
    break;

  case CRG2_VAL:
    printf("CRG2_VAL");
    break;

  case IDT_OFST:
    printf("IDT_OFST");
    break;

  case CRG1_OFST:
    printf("CRG1_OFST");
    break;

  case CRG2_OFST:
    printf("CRG2_OFST");
    break;

  case STRMEM_VAL:
    printf("STRMEM_VAL");
    break;

  case CAPET_NUMB:
    printf("CAPET_NUMB");
    break;
  case CAPET_FLOAT:
    printf("CAPET_FLOAT");
    break;
  case CAPET_CHR:
    printf("CAPET_CHR");
    break;
  case CAPET_STR:
    printf("CAPET_STR");
    break;
  case CAPET_IDT:
    printf("CAPET_IDT");
    break;
  case CAPET_CRG1:
    printf("CAPET_CRG1");
    break;
  case CAPET_CRG2:
    printf("CAPET_CRG2");
    break;
  case CAPET_STRMEM:
    printf("CAPET_STRMEM");
    break;
  case CHR_ADRS:
    printf("CHR_ADRS");
    break;
  case STR_ADRS:
    printf("STR_ADRS");
    break;

  case CAPET_EXPR:
    printf("CAPET_EXPR");
    break;

  case DAXILET_IDT:
    printf("DAXILET_IDT");
    break;

  case FARGS_NUMB:
    printf("FAGRS_NUMB");
    break;

  case FARGS_FLOAT:
    printf("FAGRS_FLOAT");
    break;

  case FARGS_CHR:
    printf("FAGRS_CHR");
    break;

  case FARGS_STR:
    printf("FAGRS_STR");
    break;

  case FARGS_IDT_VAL:
    printf("FAGRS_IDT_VAL");
    break;

  case FARGS_CRG1_VAL:
    printf("FAGRS_CRG1_VAL");
    break;

  case FARGS_CRG2_VAL:
    printf("FAGRS_CRG2_VAL");
    break;

  case FARGS_STRMEM_VAL:
    printf("FAGRS_STRMEM_VAL");
    break;

  case FARGS_EXPR_VAL:
    printf("FAGRS_EXPR_VAL");
    break;

  case FPARM_IDT_VAL:
    printf("FPARM_IDT_VAL");
    break;

  case POP_OBSTK:
    printf("POP_OBSTK");
    break;
    

case GLBMT:
    printf("GLBMT");
    break;

case FREE_SEC_DYN_STK:
	printf("FREE_SEC_DYN_STK");
	break;

case KVK:
	printf("KVK");
	break;

case KVK1:
	printf("KVK1");
	break;

case POP_FNSTK:
    printf("POP_FNSTK");
    break;

case GET_FUNC_RET_RES:
    printf("GET_FUNC_RET_RES");
    break;

case PUT_FUNC_RET_RES:
    printf("PUT_FUNC_RET_RES");
    break;

case KOD_WITH_VAR:
    printf("KOD_WITH_VAR");
    break;

case KOD_FUNK:
    printf("KOD_FUNK");
    break;

case TEXT:
    printf("TEXT");
    break;

case STANDART:
    printf("STANDART");
    break;

case STANDART_FUNK:
    printf("STANDART_FUNK");
    break;

case KVADRAT_KOK:
    printf("KVADRAT_KOK");
    break;

case STEKE_QOY:
    printf("STEKE_QOY");
    break;

case STEKE_QOY_STD:
    printf("STEKE_QOY_STD");
    break;

case STEKDEN_GOTUR:
    printf("STEKDEN_GOTUR");
    break;

case STEKDEN_GOTUR_STD:
    printf("STEKDEN_GOTUR_STD");
    break;

case SAY:
	printf("SAY");
	break;

case SAY_TIP_1:
	printf("SAY_TIP_1");
	break;

case CHECK_SAY_TIP_1:
	printf("CHECK_SAY_TIP_1");
	break;

case STEP_SAY_TIP_1:
	printf("STEP_SAY_TIP_1");
	break;

case YNT1_FUNC_ARGS:
	printf("YNT1_FUNC_ARGS");
	break;


case SAY_TIP_2:
	printf("SAY_TIP_2");
	break;

case SAY_TIP_3:
	printf("SAY_TIP_3");
	break;

case CHECK_SAY_TIP_2:
	printf("CHECK_SAY_TIP_2");
	break;

case STEP_SAY_TIP_2:
	printf("STEP_SAY_TIP_2");
	break;

case CHECK_SAY_TIP_3:
	printf("CHECK_SAY_TIP_3");
	break;

case STEP_SAY_TIP_3:
	printf("STEP_SAY_TIP_3");
	break;

case INIT_SAY_TIP_2:
	printf("INIT_SAY_TIP_2");
	break;

case INIT_SAY_TIP_3:
	printf("INIT_SAY_TIP_3");
	break;

case INCREMENT:
	printf("INCREMENT");
	break;

case DECREMENT:
	printf("DECREMENT");
	break;

case INCREMENT_OPR:
	printf("INCREMENT_OPR");
	break;

case DECREMENT_OPR:
	printf("DECREMENT_OPR");
	break;

case DAXIL_INSIDE:
	printf("DAXIL_INSIDE");
	break;

case CAPET_INSIDE:
	printf("CAPET_INSIDE");
	break;

case EGER_INSIDE:
	printf("EGER_INSIDE");
	break;

case ARRAY_INDEX:
	printf("ARRAY_INDEX");
	break;

case FCALL_VDEC:
	printf("FCALL_VDEC");
	break;

case SHERT_RIGHT:
	printf("SHERT_RIGHT");
	break;

case ASGN_RIGHT:
	printf("ASGN_RIGHT");
	break;

case SAY_INSIDE:
	printf("SAY_INSIDE");
	break;

case FUNKSIYA_ELAN:
	printf("FUNKSIYA_ELAN");
	break;

case BLOCK_INSIDE:
	printf("BLOCK_INSIDE");
	break;

case SINIF_INSIDE:
	printf("SINIF_INSIDE");
	break;

case FREE_CONTEXT:
	printf("FREE_CONTEXT");
	break;


case  NOTYET:
    printf("NOTYET");
    break;
  }
}



void sprint_tok_str(int tok, char *msg){

	memset(msg, '\0', 1024);

  switch(tok){

  case  PRG:
    sprintf(msg, "PRG");
    break;
  case  VAR:
    sprintf(msg, "VAR");
    break;
  case  TAM:
    sprintf(msg, "TAM");
    break;
  case  KESR:
    sprintf(msg, "KESR");
    break;
  case  SIMVOL:
    sprintf(msg, "SIMVOL");
    break;
  case  IDT:
    sprintf(msg, "deyishen");
    break;
  case  VERGUL:
    sprintf(msg, "`,`");
    break;
  case  NOQTEV:
    sprintf(msg, "`;`");
    break;
  case  KOD:
    sprintf(msg, "KOD");
    break;
  case  CAPET:
    sprintf(msg, "CAPET");
    break;
  case  OPNBRK:
    sprintf(msg, "`(`");
    break;
  case  STR:
    sprintf(msg, "setir");
    break;
  case  CHR:
    sprintf(msg, "simvol");
    break;
  case  CLSBRK:
    sprintf(msg, "`)`");
    break;
  case  SON:
    sprintf(msg, "SON");
    break;
  case  DOT:
    sprintf(msg, "`.`");
    break;
  case  LEXEM:
    sprintf(msg, "LEX");
    break;
  case  NUMB:
    sprintf(msg, "eded");
    break;
  case  SIGN:
    sprintf(msg, "ishare");
    break;
  case  DIG:
    sprintf(msg, "reqem");
    break;
  case  POSDIG:
    sprintf(msg, "POSDIG" );
    break;
  case  POSNUMB:
    sprintf(msg, "POSNUMB");
    break;
  case  SNUMB:
    sprintf(msg, "SNUMB");
    break;
  case  POSSIG:
    sprintf(msg, "`+`");
    break;
  case  NEGSIG:
    sprintf(msg, "`-`");
    break;
  case  SLC:
    sprintf(msg, "SLC");
    break;
  case  MLC:
    sprintf(msg, "MLC");
    break;
  case  SPS:
    sprintf(msg, "SPS");
    break;
  case  COLON:
	  sprintf(msg, "`:`");
    break;
  case  ARYOPN:
    sprintf(msg, "`[`");
    break;
  case  ARYCLS:
    sprintf(msg, "`]`");
    break;
  case  ASGN:
    sprintf(msg, "`=`");
    break;
  case  MULT:
    sprintf(msg, "`*`");
    break;
  case  DEL:
    sprintf(msg, "DEL");
    break;
  case  PRCT:
    sprintf(msg, "PRCT");
    break;
  case  XAR:
    sprintf(msg, "XAR");
    break;
  case  DAXIL:
    sprintf(msg, "DAXIL");
    break;
  case  EGER:
    sprintf(msg, "eger");
    break;
  case  NIDA:
    sprintf(msg, "NIDA");
    break;
  case  GRT:
    sprintf(msg, "`>`");
    break;
  case  LES:
    sprintf(msg, "`<`");
    break;
  case  VE:
    sprintf(msg, "ve");
    break;
  case  VEYA:
    sprintf(msg, "veya");
    break;
  case  FILESON:
    sprintf(msg, "FILESON");
    break;
  case  OPNBLK:
	  sprintf(msg, "`{`");
    break;
  case  CLSBLK:
	  sprintf(msg, "`}`");
    break;
  case  ONDA:
    sprintf(msg, "ONDA");
    break;
  case  YOXSA:
    sprintf(msg, "yoxsa");
    break;
  case  BERABER:
    sprintf(msg, "`==`");
    break;
  case  FERQLI:
    sprintf(msg, "`!=`");
    break;
  case  LESEQU:
    sprintf(msg, "LESEQU");
    break;
  case  GRTEQU:
    sprintf(msg, "GRTEQU");
    break;
  case  EMPTOK:
    sprintf(msg, "EMPTOK");
    break;
  case  SHERT:
    sprintf(msg, "shert");
    break;
  case  IFAD:
    sprintf(msg, "IFAD");
    break;
  case  ARTSIN:
    sprintf(msg, "ARTSIN");
    break;
  case  AZALSIN:
    sprintf(msg, "AZALSIN");
    break;
  case  ADDIM:
    sprintf(msg, "ADDIM");
    break;
  case  DOVR:
    sprintf(msg, "DOVR");
    break;
  case  KIMI:
    sprintf(msg, "KIMI");
    break;
  case  CRG1:
    sprintf(msg, "CRG1");
    break;
  case  CRG2:
    sprintf(msg, "CRG2");
    break;
  case  TEKRARET:
    sprintf(msg, "TEKRARET");
    break;
  case  DAN:
    sprintf(msg, "DAN");
    break;
  case  DEN:
    sprintf(msg, "DEN");
    break;

  case  CAPET_OP:
    sprintf(msg, "CAPET_OP");
    break;
  case  DOVR_OP:
    sprintf(msg, "DOVR_OP");
    break;
  case  BLOCK:
    sprintf(msg, "BLOCK");
    break;
  case  BIRTIP:
    sprintf(msg, "BIRTIP");
    break;
  case  TIPLER:
    sprintf(msg, "TIPLER");
    break;
  case  EGER_OP:
    sprintf(msg, "EGER_OP");
    break;
  case  ASGN_OP:
    sprintf(msg, "ASGN_OP");
    break;
  case  DAXIL_OP:
    sprintf(msg, "DAXIL_OP");
    break;
  case  BLOKSUZ:
    sprintf(msg, "BLOKSUZ");
    break;
  case  BLOKLU:
    sprintf(msg, "BLOKLU");
    break;
  case  KODDATALAR:
    sprintf(msg, "KODDATALAR");
    break;


  case  BLOKLUDATA:
    sprintf(msg, "BLOKLUDATA");
    break;

  case  DOVR_EXP:
    sprintf(msg, "DOVR_EXP");
    break;
  case  EGER_EXP:
    sprintf(msg, "EGER_EXP");
    break;

  case  DOVR_BLOKSUZ:
    sprintf(msg, "DOVR_BLOKSUZ");
    break;
  case  EGER_BLOKSUZ:
    sprintf(msg, "EGER_BLOKSUZ");
    break;
  case  YOXSA_BLOKSUZ:
    sprintf(msg, "YOXSA_BLOKSUZ");
    break;

  case  KODDATA:
    sprintf(msg, "emeliyyat");
    break;

  case  DOVR_BLOKLU:
    sprintf(msg, "DOVR_BLOKLU");
    break;
  case  EGER_BLOKLU:
    sprintf(msg, "EGER_BLOKLU");
    break;
  case  YOXSA_BLOKLU:
    sprintf(msg, "YOXSA_BLOKLU");
    break;
  case  EGER_TEK_BLOKSUZ:
    sprintf(msg, "EGER_TEK_BLOKSUZ");
    break;
  case  EGER_TAM_BLOKSUZ:
    sprintf(msg, "EGER_TAM_BLOKSUZ");
    break;

  case  TAM1:
    sprintf(msg, "TAM1");
    break;
  case  TAM2:
    sprintf(msg, "TAM2");
    break;
  case  KESR1:
    sprintf(msg, "KESR1");
    break;
  case  KESR2:
    sprintf(msg, "KESR2");
    break;
  case  SIMVOL1:
    sprintf(msg, "SIMVOL1");
    break;
  case  SIMVOL2:
    sprintf(msg, "SIMVOL2");
    break;
 case  VAR1:
    sprintf(msg, "VAR1");
    break;
  case  VAR2:
    sprintf(msg, "VAR2");
    break;

 case  DAXILET1:
    sprintf(msg, "DAXILET1");
    break;
  case  DAXILET2:
    sprintf(msg, "daxilet");
    break;
 case  CAPET1:
    sprintf(msg, "xaricet");
    break;
  case  CAPET2:
    sprintf(msg, "xaricet");
    break;

 case  EGER_EXP1:
    sprintf(msg, "eger");
    break;
  case  EGER_EXP2:
    sprintf(msg, "EGER_EXP2");
    break;

  case  KOD1:
    sprintf(msg, "KOD1");
    break;

  case  PRG1:
    sprintf(msg, "PRG1");
    break;


  case  BADSTR:
    sprintf(msg, "BADSTR");
    break;

 case  BADCHR:
    sprintf(msg, "BADCHR");
    break;

  case  BADIDT:
    sprintf(msg, "BADIDT");
    break;

  case  DAXILET3:
    sprintf(msg, "daxilet");
    break;

  case  EMPTYOP:
    sprintf(msg, "EMTYOP");
    break;

  case  CAPET3:
    sprintf(msg, "xarciet");
    break;

  case  EXPR:
    sprintf(msg, "EXPR");
    break;

  case  BRK:
    sprintf(msg, "BRK");
    break;


 case  FILEBEG:
    sprintf(msg, "FILEBEG");
    break;

 case  FLOAT_HALF:
    sprintf(msg, "FLOAT_HALF");
    break;


 case  FLOAT:
    sprintf(msg, "FLOAT");
    break;

 case  FUNKSIYA:
    sprintf(msg, "FUNKSIYA");
    break;


 case  FUNK_ELAN:
    sprintf(msg, "funksiya elani");
    break;


 case  FUNK_BODY:
    sprintf(msg, "FUNK_BODY");
    break;


 case  EMPBRK:
    sprintf(msg, "EMPBRK");
    break;

 case  FE1:
    sprintf(msg, "FE1");
    break;

 case  FE2:
    sprintf(msg, "FE2");
    break;

 case  FUNK:
    sprintf(msg, "FUNK");
    break;

 case  FUNKS:
    sprintf(msg, "FUNKS");
    break;

 case  FCALL:
    sprintf(msg, "FCALL");
    break;

 case  FCALL1:
    sprintf(msg, "FCALL1");
    break;

 case  VDEC:
    sprintf(msg, "VDEC");
    break;


 case  DUMMY:
   sprintf(msg, " ");  /* don't print dummy tokens, they play no role during parsing */
    break;

  case  FDECL:
    sprintf(msg, "FDECL");
    break;
	
  case  PRG2:
    sprintf(msg, "PRG2");
    break;

  case  QAYTAR:
    sprintf(msg, "QAYTAR");
    break;

 case  QAYTAR1:
    sprintf(msg, "QAYTAR1");
    break;

 case  NEQEDERKI:
    sprintf(msg, "NEQEDERKI");
    break;

 case  NEQEDERKI1:
    sprintf(msg, "NEQEDERKI1");
    break;

 case  NEQEDERKI2:
    sprintf(msg, "NEQEDERKI2");
    break;

 case  DAVAMET:
    sprintf(msg, "DAVAMET");
    break;

 case  DAYAN:
    sprintf(msg, "DAYAN");
    break;

 case  DAVAMET1:
    sprintf(msg, "DAVAMET1");
    break;

 case  DAYAN1:
    sprintf(msg, "DAYAN1");
    break;

 case  SEC:
    sprintf(msg, "SEC");
    break;

 case  SEC_OP:
    sprintf(msg, "SEC_OP");
    break;

 case  HAL:
    sprintf(msg, "HAL");
    break;

 case  HAL1:
    sprintf(msg, "HAL1");
    break;

 case  HAL2:
    sprintf(msg, "HAL2");
    break;

 case  HAL3:
    sprintf(msg, "HAL3");
    break;

 case  SEC1:
    sprintf(msg, "SEC1");
    break;

 case  SUSMA:
    sprintf(msg, "SUSMA");
    break;

case  START:
    sprintf(msg, "S");
    break;

case  STRUKT:
    sprintf(msg, "STRUKT");
    break;

case  STRUKT1:
    sprintf(msg, "STRUKT1");
    break;

case  STRUKT2:
    sprintf(msg, "STRUKT2");
    break;

case  STRUKT3:
    sprintf(msg, "STRUKT3");
    break;

case  YNT1:
    sprintf(msg, "ifade");
    break;

case  YNT2:
    sprintf(msg, "YNT2");
    break;

case  STRMEM:
    sprintf(msg, "STRMEM");
    break;


case  YNTS:
    sprintf(msg, "YNTS");
    break;

case  SINIF:
    sprintf(msg, "SINIF");
    break;

case  QORUNAN:
    sprintf(msg, "QORUNAN");
    break;

case  UMUMI:
    sprintf(msg, "UMUMI");
    break;

case  MEXSUSI:
    sprintf(msg, "MEXSUSI");
    break;

case  SINIF_HEAD:
    sprintf(msg, "SINIF_HEAD");
    break;

case  SINIF_HEAD_START:
    sprintf(msg, "SINIF_HEAD_START");
    break;

case  SINIF_BODY:
    sprintf(msg, "SINIF_BODY");
    break;

case  SINIF_DECL:
    sprintf(msg, "SINIF_DECL");
    break;

case  SINIF_TYPE:
    sprintf(msg, "SINIF_TYPE");
    break;

case  YENI_TIP:
    sprintf(msg, "YENI_TIP");
    break;

case  SNMEM:
    sprintf(msg, "SNMEM");
    break;



case  INIT_DOVR:
    sprintf(msg, "INIT_DOVR");
    break;

case  CHECK_DOVR:
    sprintf(msg, "CHECK_DOVR");
    break;

case  STEP_DOVR:
    sprintf(msg, "STEP_DOVR");
    break;

case NEQ_TOP:
    sprintf(msg, "NEQ_TOP");
    break;

case SUS1:
    sprintf(msg, "SUS1");
    break;

case HAL1S:
    sprintf(msg, "HAL1S");
    break;

  case FARGS:
    sprintf(msg, "FARGS");
    break;

  case FPARM:
    sprintf(msg, "FPARM");
    break;

  case FCALL_BEG:
    sprintf(msg, "FCALL_BEG");
    break;


  case PUT_RET_ADDR:
    sprintf(msg, "PUT_RET_ADDR");
    break;


  case IDT_ADRS:
    sprintf(msg, "IDT_ADRS");
    break;

  case IDT_VAL:
    sprintf(msg, "IDT_VAL");
    break;

  case CRG1_ADRS:
    sprintf(msg, "CRG1_ARDS");
    break;

  case CRG2_ADRS:
    sprintf(msg, "CRG2_ADRS");
    break;

  case CRG1_VAL:
    sprintf(msg, "CRG1_VAL");
    break;

  case CRG2_VAL:
    sprintf(msg, "CRG2_VAL");
    break;

  case IDT_OFST:
    sprintf(msg, "IDT_OFST");
    break;

  case CRG1_OFST:
    sprintf(msg, "CRG1_OFST");
    break;

  case CRG2_OFST:
    sprintf(msg, "CRG2_OFST");
    break;

  case STRMEM_VAL:
    sprintf(msg, "STRMEM_VAL");
    break;

  case CAPET_NUMB:
    sprintf(msg, "CAPET_NUMB");
    break;
  case CAPET_FLOAT:
    sprintf(msg, "CAPET_FLOAT");
    break;
  case CAPET_CHR:
    sprintf(msg, "CAPET_CHR");
    break;
  case CAPET_STR:
    sprintf(msg, "CAPET_STR");
    break;
  case CAPET_IDT:
    sprintf(msg, "CAPET_IDT");
    break;
  case CAPET_CRG1:
    sprintf(msg, "CAPET_CRG1");
    break;
  case CAPET_CRG2:
    sprintf(msg, "CAPET_CRG2");
    break;
  case CAPET_STRMEM:
    sprintf(msg, "CAPET_STRMEM");
    break;
  case CHR_ADRS:
    sprintf(msg, "CHR_ADRS");
    break;
  case STR_ADRS:
    sprintf(msg, "STR_ADRS");
    break;

  case CAPET_EXPR:
    sprintf(msg, "CAPET_EXPR");
    break;

  case DAXILET_IDT:
    sprintf(msg, "DAXILET_IDT");
    break;

  case FARGS_NUMB:
    sprintf(msg, "FAGRS_NUMB");
    break;

  case FARGS_FLOAT:
    sprintf(msg, "FAGRS_FLOAT");
    break;

  case FARGS_CHR:
    sprintf(msg, "FAGRS_CHR");
    break;

  case FARGS_STR:
    sprintf(msg, "FAGRS_STR");
    break;

  case FARGS_IDT_VAL:
    sprintf(msg, "FAGRS_IDT_VAL");
    break;

  case FARGS_CRG1_VAL:
    sprintf(msg, "FAGRS_CRG1_VAL");
    break;

  case FARGS_CRG2_VAL:
    sprintf(msg, "FAGRS_CRG2_VAL");
    break;

  case FARGS_STRMEM_VAL:
    sprintf(msg, "FAGRS_STRMEM_VAL");
    break;

  case FARGS_EXPR_VAL:
    sprintf(msg, "FAGRS_EXPR_VAL");
    break;

  case FPARM_IDT_VAL:
    sprintf(msg, "FPARM_IDT_VAL");
    break;

  case POP_OBSTK:
    sprintf(msg, "POP_OBSTK");
    break;
    

case GLBMT:
    sprintf(msg, "GLBMT");
    break;

case FREE_SEC_DYN_STK:
	sprintf(msg, "FREE_SEC_DYN_STK");
	break;

case KVK:
	sprintf(msg, "KVK");
	break;

case KVK1:
	sprintf(msg, "KVK1");
	break;

case POP_FNSTK:
    sprintf(msg, "POP_FNSTK");
    break;

case GET_FUNC_RET_RES:
    sprintf(msg, "GET_FUNC_RET_RES");
    break;

case PUT_FUNC_RET_RES:
    sprintf(msg, "PUT_FUNC_RET_RES");
    break;

case KOD_WITH_VAR:
    sprintf(msg, "KOD_WITH_VAR");
    break;

case KOD_FUNK:
    sprintf(msg, "emeliyyat ve ya funksiya");
    break;

case TEXT:
    sprintf(msg, "emeliyyat");
    break;

case STANDART:
    sprintf(msg, "STANDART");
    break;

case STANDART_FUNK:
    sprintf(msg, "STANDART_FUNK");
    break;

case KVADRAT_KOK:
    sprintf(msg, "KVADRAT_KOK");
    break;

case STEKE_QOY:
    sprintf(msg, "STEKE_QOY");
    break;

case STEKE_QOY_STD:
    sprintf(msg, "STEKE_QOY_STD");
    break;

case STEKDEN_GOTUR:
    sprintf(msg, "STEKDEN_GOTUR");
    break;

case STEKDEN_GOTUR_STD:
    sprintf(msg, "STEKDEN_GOTUR_STD");
    break;

case SAY:
	sprintf(msg, "say");
	break;

case SAY_TIP_1:
	sprintf(msg, "say");
	break;

case CHECK_SAY_TIP_1:
	sprintf(msg, "CHECK_SAY_TIP_1");
	break;

case STEP_SAY_TIP_1:
	sprintf(msg, "STEP_SAY_TIP_1");
	break;

case YNT1_FUNC_ARGS:
	sprintf(msg, "YNT1_FUNC_ARGS");
	break;


case SAY_TIP_2:
	sprintf(msg, "say");
	break;

case OPERATOR:
	sprintf(msg, "operator");
	break;

case SAY_TIP_3:
	sprintf(msg, "say");
	break;

case CHECK_SAY_TIP_2:
	sprintf(msg, "CHECK_SAY_TIP_2");
	break;

case STEP_SAY_TIP_2:
	sprintf(msg, "STEP_SAY_TIP_2");
	break;

case CHECK_SAY_TIP_3:
	sprintf(msg, "CHECK_SAY_TIP_3");
	break;

case STEP_SAY_TIP_3:
	sprintf(msg, "STEP_SAY_TIP_3");
	break;

case INIT_SAY_TIP_2:
	sprintf(msg, "INIT_SAY_TIP_2");
	break;

case INIT_SAY_TIP_3:
	sprintf(msg, "INIT_SAY_TIP_3");
	break;

case INCREMENT:
	sprintf(msg, "INCREMENT");
	break;

case DECREMENT:
	sprintf(msg, "DECREMENT");
	break;

case INCREMENT_OPR:
	sprintf(msg, "INCREMENT_OPR");
	break;

case DECREMENT_OPR:
	sprintf(msg, "DECREMENT_OPR");
	break;

case DAXIL_INSIDE:
	sprintf(msg, "DAXIL_INSIDE");
	break;

case CAPET_INSIDE:
	sprintf(msg, "CAPET_INSIDE");
	break;

case EGER_INSIDE:
	sprintf(msg, "EGER_INSIDE");
	break;

case ARRAY_INDEX:
	sprintf(msg, "ARRAY_INDEX");
	break;

case FCALL_VDEC:
	sprintf(msg, "FCALL_VDEC");
	break;

case SHERT_RIGHT:
	sprintf(msg, "SHERT_RIGHT");
	break;

case ASGN_RIGHT:
	sprintf(msg, "ASGN_RIGHT");
	break;

case SAY_INSIDE:
	sprintf(msg, "SAY_INSIDE");
	break;

case FUNKSIYA_ELAN:
	sprintf(msg, "FUNKSIYA_ELAN");
	break;

case BLOCK_INSIDE:
	sprintf(msg, "BLOCK_INSIDE");
	break;

case SINIF_INSIDE:
	sprintf(msg, "SINIF_INSIDE");
	break;

case FREE_CONTEXT:
	sprintf(msg, "FREE_CONTEXT");
	break;


case  NOTYET:
    sprintf(msg, "NOTYET");
    break;
  }
}



void tokenize(char *src){
    get_tokens(src);
	init_hash_ids();
	//print_tokens(); 
}
