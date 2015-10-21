
#include "esrin.h"

/* ACAR sozler  */
struct token lexems[]={
  {"proqram", PRG},
  {"var", VAR},
  //  {"tam", TAM},
  //  {"kesr", KESR},
  //  {"simvol",SIMVOL},
  {",",VERGUL},
  {";",NOQTEV},
  {"kod", KOD}, 
  {"capet", CAPET},
  {"(",OPNBRK},
  {")",CLSBRK},
  {"son", SON},
  {".", DOT},
  {"daxilet", DAXIL},
  {"eger", EGER},
  {"onda", ONDA},
  {"yoxsa", YOXSA},
  {"ve", VE},
  {"veya", VEYA},
  {"dovr", DOVR},
  {"artsin", ARTSIN },
  {"azalsin", AZALSIN},
  {"addim", ADDIM},
  {"tekraret", TEKRARET},
  {"kimi", KIMI},
  {"dan", DAN},
  {"den", DEN},
  {"ye",YE},
  {"ya",YA},
  {"e",EE},
  {"a",AA},
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
  {"serbest",SERBEST},
  {"qorunan",QORUNAN},
  {"kvk",KVK}
/* !!!!!!!!!!!! DON'T FORGET UPDATE lxmc below !!!!!!!!!!  */
};

int lxmc = 42;


#define MAX_LINES 10000
char * lines[MAX_LINES];

int lnct = 0;

void init_lines( char *name){

	char *p, *q, *text, *end, **s;
	int fd, size,k;
    
  FILE *f = fopen(name, "rb");
  fseek(f,0,SEEK_END);
  size = ftell(f);
  fseek(f,0,SEEK_SET);

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



void sherhleri_sil(char *text){

    int k = strlen(text);

    char *p,*end;

    end = text + k;

    p = text;

    for (p=text; p<end; p++){
	if  ((*p=='/') && (*(p+1)=='/')){
		while ((*(p+1)!='\n') && (p<end)) {
			 *p = ' ';
			 p++;
			}
		*p = ' ';
	}

	if  ((*p=='/') && (*(p+1)=='*')){
		while (!((*p=='*') && (*(p+1)=='/'))&& (p<end)) {
			 *p = ' ';
			 p++;
			}
		*p = ' ';
		*(p+1) = ' ';
		p+=2;
	}
     }
}


/* parse text and get pos and numb of new lines */



void print_tokens(){

  int i;

  printf("Sizin tokenler\n");
  
  for (i=0; i<tks; i++){
    printf("i = %d, %s\t\t%d\t",i, tokens[i].tok, tokens[i].id);
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

  if (*p == '"')
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

  if (*p == '+')
    return POSSIG;

  if (*p == '-')
    return NEGSIG;

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

  return NOTYET;


}


char *fin_spc(char *p){

  while (p<end && ( *p==' ' ||  *p=='\t' || *p == '\n' || *p == '\r' )){
    if (*p == '\r' && *(p+1) == '\n'){
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
    if (*p == '\n'){
      cur_row++;
    last_col = p;
    }
	 p++;
  }

  return p+2;

}

void get_tokens(char *name){

  char *p, *q, tmp[1024];
  int fd,k, size, nmb;

  FILE *f = fopen(name, "rb");
  fseek(f,0,SEEK_END);
  size = ftell(f);
  fseek(f,0,SEEK_SET);

  text = (char *)malloc(size+1);
  memset(text, '\0', size+1);

  fread(text, size, 1, f);
  fclose(f);


  end = text + strlen(text);

	/*sherhleri_sil(text);*/

  //file_name = name;

  p = text;
  last_col = text;

  tokens[0].id = FILEBEG;
  tks++;


  while (p<end){
    
    k = wtch(p);

    switch(k){
 
     case SPS:
       p = fin_spc(p);
       break;
    case STR:
      q = fin_str(p);
      if (q>=end){
	get_new_token(p,q,BADSTR, cur_row, p - last_col);
	goto out;
      }
      if (*q == '\n'){
	get_new_token(p,q,BADSTR, cur_row, p - last_col);
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
    case CHR:
      q = fin_chr(p);
      if (q>=end){
	/* simvolu baglayan dirnag yaddan cixib  */
	get_new_token(p,q,BADCHR, cur_row, p - last_col);
	goto out;
      }
      if (*q == '\n'){
	/* simvolu baglayan dirnag yaddan cixib  */
	get_new_token(p,q,BADCHR, cur_row, p - last_col);
	p = q+1;
	cur_row++;
	last_col = p;
	break;
      }

      if (q - 1 == p + 1){
	printf("Sehv: setir %d, sutun %d: \'\'  tek dirnaq arasinda hecne yoxdur. \n", cur_row, p - last_col);
	err_cnt++;
	p = q;
	break;
      }

      if (q-p > 3){

	/*
	if ((q - p == 4) && *(p+1) == '\\' && *(p+2) == 'y'   ){
	     get_new_token(p+1, p+3, CHR, cur_row, p - last_col);
	     p = q;
	     break;
	}
	*/

	if ((q - p == 4) && *(p+1) == '\\'){
	  switch(*(p+2)){
	  case 'y':
	    *(p+1) = '\n';
	    get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
	    p = q;
	    break;
	  case 't':
	    *(p+1) = '\t';
	    get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
	    p = q;
	    break;
	  case '\'':
	    *(p+1) = '\'';
	    get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
	    p = q;
	    break;
	  case '\"':
	    *(p+1) = '\"';
	    get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
	    p = q;
	    break;
	  case '\0':
	    *(p+1) = '\0';
	    get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
	    p = q;
	    break;
	  }
	}


	if (p == q)
	  break;	  
	

	nmb = q - p;
	sprintf(tmp, p, nmb  );
	*(tmp + nmb) = '\0';
	printf("Xeberdarliq: setir %d, sutun %d: %s tek dirnaq icinde bir nece simvol. \n", cur_row, p - last_col, tmp);
	printf("Ilk simvola qeder qisaldilir\n");
      }

      get_new_token(p+1, p+2, CHR, cur_row, p - last_col);
      p = q;
      break;       
    case IDT:
      q = fin_idt(p);
      get_new_token(p,q, LEXEM, cur_row, p - last_col);
      p = q;
      break;

    case DIG:
      q = fin_numb(p);

      if (*q == '.'){
	int tmp  = wtch(q+1);
	if (tmp != DIG){
	  get_new_token(p,q+1, FLOAT_HALF, cur_row, p - last_col);
	  p = q+1;
	  break;
	}
	else{
	  q = fin_numb(q+1);
	  get_new_token(p,q, FLOAT, cur_row, p - last_col);
	  p = q;
	  break;
	}
      }
	
       get_new_token(p,q, NUMB, cur_row, p - last_col);
      p = q;
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
    printf("DAXILET");
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
  case  BLOK:
    printf("BLOK");
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

  case  YA:
    printf("YA");
    break;
  case  YE:
    printf("YE");
    break;
  case  AA:
    printf("A");
    break;
  case  EE:
    printf("E");
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
    printf("EGREXP1");
    break;
  case  EGER_EXP2:
    printf("EGREXP2");
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

case  SERBEST:
    printf("SERBEST");
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
    printf("IDT");
    break;
  case  VERGUL:
    printf("VERGUL");
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
    printf("DAXILET");
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
  case  BLOK:
    printf("BLOK");
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

  case  YA:
    printf("YA");
    break;
  case  YE:
    printf("YE");
    break;
  case  AA:
    printf("A");
    break;
  case  EE:
    printf("E");
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
    printf("EGREXP1");
    break;
  case  EGER_EXP2:
    printf("EGREXP2");
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
    printf("ifade");
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

case  SERBEST:
    printf("SERBEST");
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

case GLBMT:
    printf("GLBMT");
    break;

  case  NOTYET:
    printf("NOTYET");
    break;
  }
}


void tokenize(char *src){
    get_tokens(src);
	init_hash_ids();
	print_tokens(); 
}
