/* Lisence GPL V3
   Ahmed Sadikhov
   ahmed.sadikhov@gmail.com
   program to build symbol tables
   */




  #include "esrin.h"
#include "sehvler.h"

extern void print_normal(struct tok_list_el *tll, char *msg);
extern void reset_error_counter();
extern void check_symtab_errors();

struct ttable ttable_cont, *tt;
	struct func_table *glbft;


 /* Programs global vars  */
struct var_table *glbmt;

void  get_sntip_and_fid(int sntip, unsigned long hid, int *fsntip, int *fid);
int get_tip_size(int tip);

int get_tip_id(unsigned long hid){

  int i;

  for (i=0; i<tt->count; ++i)
    if (tt->tips[i].hid == hid)
      return  tt->tips[i].id;

  return -1; /* not such tip, error, possibly fatal  */
}


/* get strukt tip size  */
int get_tip_size_str(int tip){

  struct var_table *mt;
  int i,k, d1, d2;

  k = 0;
  mt = &tt->tips[tip].mt;

  if (mt == NULL)
    return 0;

  if (mt->count == 0)
    return 0;

  for (i=0; i<mt->count; ++i){
      d1 = mt->vars[i].tok->d1;
      d2 = mt->vars[i].tok->d2;
      k += d1*d2*get_tip_size(mt->vars[i].tip);
  }

  return k;
}

int get_tip_size(int tip){

  switch (tip){
  case 1: /* int tip, we use long int  */
    return sizeof(long int);
  case 2:
    return sizeof(long double);
  case 3:
    return sizeof(char);
    /* calculating of struct tip sizes to be implemented ...  */
  default:
    return get_tip_size_str(tip);
  }

}

void init_tips_size(struct ttable *tt){

  int i;

  /* standard types  */
  for (i=1; i<=3; ++i)
    tt->tips[i].size = get_tip_size(i);

  /* user types  */
  for (i=USR_TIP_START; i<tt->count; ++i)
    tt->tips[i].size = get_tip_size(i);
}

void print_memb(struct var *var){
  printf("\t\t\t   %s\td1\t%d\td2\t%d\t",var->tok->tok, var->tok->d1, var->tok->d2);
  print_tok(var->as);
  printf("\tofst\t %d\ttip  %d\t",var->tok->ofst, var->tip);
  if (var->mbtip == MEXSUSI ||\
      var->mbtip == QORUNAN ||\
      var->mbtip == UMUMI)
    print_tok(var->mbtip);
  printf("\n");
}

void print_var(struct var *var){
  printf("\t\t\t   %s\td1\t%d\td2\t%d\t",var->tok->tok, var->tok->d1, var->tok->d2);
  print_tok(var->as);
  printf("\tofst\t %d\ttip  %d\n",var->tok->ofst, var->tip);
}

void print_parms(struct par_list *pl){

  int i;

  if (pl->count == 0)
    return ;

  printf("\n\t\t\t Funksiya Parametrleri\n");

  for(i=0; i<pl->count; ++i)
    print_var(pl->pars[i]);
}


void print_fargs_table(struct func_args_table *fat){

  struct func_args_list *fal;
  int i, j, sntip;

  if (fat == NULL)
    return;

  if (fat->count == 0)
    return;

  printf("\n\t\t\tFunksiya Arqumentleri Cedveli\n");

  for (i=0; i<fat->count; ++i){
    fal = &fat->fargs[i];
    sntip = fal->tok->sntip;
    printf("\n\t\t\t\tvalideyn\t %s\tfunk %s\thedd %d\t fid %d\t", tt->tips[sntip].name, fal->tok->tok, fal->memb,  fal->tok->fid);
    for (j=0; j<fat->fargs[i].count; ++j){

      switch(fat->fargs[i].fels[j].tll->id){
	

      }

      printf("%s\t srsh %d\t", fat->fargs[i].fels[j].tll->tok->tok, fat->fargs[i].fels[j].tll->tok->ofst);
    print_tok(fat->fargs[i].fels[j].usg);
    printf(" , ");
    }  
  }

  printf("\n\n");

}


void print_dec_table(struct var_table *dt){

  int i;
 
  if (dt->count == 0)
    return;

  printf("\n\t\t\tFunksiyanin Lokal Deyishenleri\n");

  for (i=0; i<dt->count; ++i)
    print_var(&dt->vars[i]);
}

void print_members(int tip){

  struct var_table *mt;
  int i, mtip;

  mt = &tt->tips[tip].mt;

  if (mt == NULL)
    return;

  if (mt->count == 0)
    return;

    printf("\n\t\t Deyishen Heddleri\n");

  for (i=0; i<mt->count; ++i)
    print_memb(&mt->vars[i]);
}



void print_usg_table(struct usg_table *ut){

  int i;

  printf("\n\t\t\tDeyishen Heddlerin Istifadesi\n");

  for (i=0; i<ut->count; ++i){
    printf("\t\t\t   %s\t", ut->usgs[i].tok->tok);
    //    printf("\td1 %d\td2 %d\tusg ",ut->usgs[i].tok->d1,ut->usgs[i].tok->d2);
    print_tok(ut->usgs[i].as);
    printf("\tlgm\t");
    switch(ut->usgs[i].tok->lgm){
    case LOCAL:
      printf("LOCAL\t");
      break;
    case MEMBER:
      printf("MEMBER\t");
      break;
    case GLOBAL:
      printf("GLOBAL\t");
      break;
    }
    printf("srsh\t%d\t", ut->usgs[i].tok->ofst);
    printf("tip\t%s\n", tt->tips[ut->usgs[i].tok->tip].name);
    
    /*    printf(" ofst %d, memb %d, ptip  %d,\tfa\t%d",usg_tab->usgs[i].fid, usg_tab->usgs[i].tok->fid, usg_tab->usgs[i].tok->tip, usg_tab->usgs[i].tok->ofst, usg_tab->usgs[i].memb, usg_tab->usgs[i].ptip, usg_tab->usgs[i].fa  );
     */
    /* printing parent for members  */
    /*
    if ( usg_tab->usgs[i].ptll != NULL){
      if ( usg_tab->usgs[i].ptll->id == IDT)
	printf("  %s  ", usg_tab->usgs[i].ptll->tok->tok);
      else
	printf("  %s  ", usg_tab->usgs[i].ptll->childs[0]->tok->tok);
    }
    */
    printf("\n");
  }
}



void print_f_usgs(struct f_usgs *f_ut){

  int i;

  printf("\n\t\t\tFunksiya cagirilmasi\n");

  for (i=0; i<f_ut->count; ++i){
	  printf("\t\t\t   %s\t%d,%d", f_ut->usgs[i].tok->tok, f_ut->usgs[i].tok->row, f_ut->usgs[i].tok->col);
    //    printf("\td1 %d\td2 %d\tusg ",ut->usgs[i].tok->d1,ut->usgs[i].tok->d2);
	  if (f_ut->usgs[i].qyt_val)
		  printf(" returns value");
	  else
		  printf(" does not return value");
      printf("\n");
  }
}


void print_f_qyts(struct qyt_list *ql){

  int i;

  printf("\n\t\t\tFunksiya qaytar siyahisi\n");

  for (i=0; i<ql->count; ++i){
	  printf("\t\t\t   %s\t%d,%d", ql->qyts[i].tok->tok, ql->qyts[i].tok->row, ql->qyts[i].tok->col);
    //    printf("\td1 %d\td2 %d\tusg ",ut->usgs[i].tok->d1,ut->usgs[i].tok->d2);
	  if (ql->qyts[i].qyt_res)
		  printf(" returns value");
	  else
		  printf(" does not return value");
      printf("\n");
  }
}


void print_funcs(int tip){

  struct func_table *ft;
  int i, ptip;
  char *s;
  unsigned long fhid;

  ft = &tt->tips[tip].ft;

  if (ft == NULL)
    return;

   if (ft->count == 0)
    return;

  //  if (ft->count == 0)
  //  printf("ft->count = %d\n", ft->count);
  

  printf("\n\t\tFunksiya Heddleri\n");

  for (i=0; i<ft->count; ++i){
    /* print funcs parent name and acs rght if it decld insd sinif  */ 
    printf("\t\t\t");
    print_tok(ft->funcs[i].mbtip);
    printf("\t%s() \n",ft->funcs[i].tok->tok);
    print_parms(&ft->funcs[i].pl);
    print_dec_table(&ft->funcs[i].dt);
    print_usg_table(&ft->funcs[i].ut);
    print_fargs_table(&ft->funcs[i].fat);
	print_f_usgs(&ft->funcs[i].f_ut);
	print_f_qyts(&ft->funcs[i].ql);
  }

}

void print_ttable(struct ttable *ttab){

  int i;

  printf("Tipler\n");

  /* standart tipleri cap elemirik  */

  for (i=0; i<ttab->count; ++i){
    if (strcmp(ttab->tips[i].name, "\0")!=0)
      printf("\n\t%s\tid %d,\tolcu %d\n",ttab->tips[i].name, ttab->tips[i].id, ttab->tips[i].size);
      print_members(i);
      print_funcs(i);
  }
}



void add_fargs_list_el(struct func_args_list *fls, struct tok_list_el *tll, int usg){
  fls->fels[fls->count].tll = tll;
  fls->fels[fls->count].usg = usg;
  fls->count++;
}

void build_fargs_list(struct func_args_list *fls, struct tok_list_el *tll){

  int i;

  if (tll->id == FCALL)
    return;

   switch(tll->id){
   case IDT:
     add_fargs_list_el(fls, tll, IDT);
    return;
   case CRG1:
     add_fargs_list_el(fls, tll, CRG1);
    return;
   case CRG2:
     add_fargs_list_el(fls, tll, CRG2);
    return;
   case STRMEM:
     add_fargs_list_el(fls, tll, STRMEM);
    return;
   case NUMB:  /* bes expr, fcall1 ????  */
   case FLOAT:
   case SIMVOL:
   case EXPR:
     add_fargs_list_el(fls, tll, tll->id);
    return;
   }
    
  for (i=0; i<tll->cld_cnt; ++i)
    build_fargs_list(fls, tll->childs[i]);
}   


void add_fargs_table(struct tok_list_el *tll, struct tok_list_el *ptll, int sntip, int fid, int member){

  struct func_args_table *fat;
  struct token *tok;

    tok = tll->childs[0]->childs[0]->tok;
    //  get_sntip_and_fid(sntip, tok->hid,  &tok->sntip, &tok->fid);
    fat = &tt->tips[sntip].ft.funcs[fid].fat;
    // fat->fargs[fat->count].fid = tok->fid;
    fat->fargs[fat->count].sntip = sntip;
    fat->fargs[fat->count].ptll = ptll;
    fat->fargs[fat->count].memb = member;
    fat->fargs[fat->count].count = 0;
    fat->fargs[fat->count].tok = tok;

    build_fargs_list(&fat->fargs[fat->count], tll);
    tok->fargsid = fat->count;
    fat->count++;
}

void tip_elave_et(char *name, int id ){

  unsigned long hid;

  hash((unsigned char *)name, &hid); 

  tt->tips[tt->count].id = id;
  tt->tips[tt->count].hid = hid;
  tt->tips[tt->count].size = 0;
  //tt->tips[tt->count].mt = NULL;
  tt->tips[tt->count].mt.count = 0;
//  tt->tips[tt->count].ft = NULL;
  tt->tips[tt->count].marea = NULL;
  strcpy(tt->tips[tt->count].name, name);

  tt->count++;
}


void standart_tipleri_elave_et(void){

  int i;

  /* add standard types  */   
    tip_elave_et("tam", 1);
    tip_elave_et("kesr", 2);
    tip_elave_et("simvol", 3);

  /* tips from 4 - to 200 reserved for standart types
     and other purposes  */
   for (i=4; i<USR_TIP_START; ++i)
     tip_elave_et("\0",i);
}

/* 
   init types table
   0 - not used
   1 - 100 Standart tipler
   101-200 Reserv Types
   > 200   User Types
  */
void init_tt(){
   tt = &ttable_cont;
   tt->count = 1;
   standart_tipleri_elave_et();
}

void init_glbl(){

	/* programs global funcs table  */


  /* global_funcs  */
//  tt->tips[GLB_TIP].ft = (struct func_table *)malloc(sizeof(struct func_table));
  //glbft = (struct func_table *)malloc(sizeof(struct func_table));
  glbft = &(tt->tips[GLB_TIP].ft);
//  glbft = tt->tips[GLB_TIP].ft;  

  /* first function is main funkstion, reserve place for it
     members will be set during build  */
//  tt->tips[GLB_TIP].ft->count = 1;
	glbft->count = 1; /* previouslu we assigned this field to 1 main func  */
  

  /* global vars  */
 // tt->tips[GLB_TIP].mt = (struct var_table *)malloc(sizeof(struct var_table));  
  glbmt = &tt->tips[GLB_TIP].mt;  

  strcpy(tt->tips[GLB_TIP].name, "GLBPRG\0");
  tt->tips[GLB_TIP].id = GLB_TIP;
  tt->count++;
}

void init_symtab(){

  init_tt();
  init_glbl();
  //init_func_table(&func_table_cont, &func_tab);
  //init_func_args_table(&fat_cont, &fat);
}


void add_dec_table(struct func_table *ft, struct var_table *dt, struct tok_list_el *tll, int tip, int as, int fid, int father){

  dt->vars[dt->count].tip = tip;
  dt->vars[dt->count].tok = tll->tok;
  dt->vars[dt->count].as  = as;
  dt->vars[dt->count].fid = fid;
  dt->vars[dt->count].tll = tll;

  /* add dec_list_el to func parlist if so  */
  if (father == FUNK_ELAN){
    ft->funcs[fid].pl.pars[ft->funcs[fid].pl.count] = &(dt->vars[dt->count]);
    ft->funcs[fid].pl.count++;
  }

  /* besides keeping data in symtab we also update token inf of func pars
     which is used for passing args to func  */
  dt->vars[dt->count].tok->lgm = LOCAL;
  dt->vars[dt->count].tok->tip = tip;
  /* ofst is set by build_vars(). TODO: set tip, ofst and lgm for func pars neatly  */

      
  dt->count++;

}


void check_tip(int tip, struct token *tok){

	if (tip <= 0 )
		print_type_not_def_err(tok);
}

/* traverse parse tree and build variable declaration table  */
void build_dec_table_yeni(struct func_table *ft, struct var_table *dt, struct tok_list_el  *tll , int fid, int *father, int tip){

  int i, id = tll->id;
 
 
  if (tll->id == YNT2 && tll->childs[0]->id == YNT1){
	tip = get_tip_id(tll->childs[0]->childs[0]->tok->hid);
	check_tip(tip, tll->childs[0]->childs[0]->tok);
  }
 
 
   
 if (tll->id == YNT1 && tll->childs[0]->id == IDT){ 
   tip = get_tip_id(tll->childs[0]->tok->hid);
   check_tip(tip, tll->childs[0]->tok);

	switch(tll->childs[1]->id){
	case IDT:
	  add_dec_table(ft, dt, tll->childs[1], tip, IDT, fid, *father);
	  break;
	case CRG1:
 	  add_dec_table(ft, dt, tll->childs[1]->childs[0], tip, CRG1, fid, *father);
	  break;
	case CRG2:
 	  add_dec_table(ft, dt, tll->childs[1]->childs[0], tip, CRG2, fid, *father);
	  break;
	}
 }
 if (tll->id == YNTS && tll->cld_cnt == 3  ){ 
	switch(tll->childs[2]->id){
	case IDT:
	  add_dec_table(ft, dt, tll->childs[2], tip, IDT, fid, *father);
 	  break;
	case CRG1:
 	  add_dec_table(ft, dt, tll->childs[2]->childs[0], tip, CRG1, fid, *father);
	  break;
	case CRG2:
 	  add_dec_table(ft, dt, tll->childs[2]->childs[0], tip, CRG2, fid, *father);
	  break;
	}
 }
 if (tll->id == YNTS && tll->cld_cnt == 2  ){ 
	switch(tll->childs[1]->id){
	case IDT:
	  add_dec_table(ft, dt, tll->childs[1], tip, IDT, fid, *father);
	  break;
	case CRG1:
 	  add_dec_table(ft, dt, tll->childs[1]->childs[0], tip, CRG1, fid, *father);
	  break;
	case CRG2:
 	  add_dec_table(ft, dt, tll->childs[1]->childs[0], tip, CRG2, fid, *father);
	  break;
	}
 }
 
 if (tll->id == FUNK_ELAN)
   *father = FUNK_ELAN;

 if (tll->id == BLOCK)
   *father = FUNK_BODY;

 for (i=0; i<tll->cld_cnt; ++i)
     build_dec_table_yeni(ft, dt, tll->childs[i], fid, father, tip);

}


void override_func_table(struct tok_list_el *kod, int i, struct func_table *ft, struct token *tkn,  int ptip, int mbtip){

  ft->funcs[i].kod = kod;
  ft->funcs[i].tok = tkn;
//  ft->funcs[i].hid  = tkn->hid;
  ft->funcs[i].qytval = 0;  
  ft->funcs[i].hasqyt = 0;
  ft->funcs[i].id  = i;
  ft->funcs[i].ptip = ptip;
  ft->funcs[i].mbtip = mbtip;
  //ft->funcs[i].fat = (struct func_args_table *)malloc(sizeof(struct func_args_table));
  ft->funcs[i].fat.count = 0;
  //  (ft->size)++;
}

void add_func_table(struct tok_list_el *kod, struct token *tkn, int ptip, int mbtip){

  struct func_table *ft = &tt->tips[ptip].ft;

  ft->funcs[ft->count].kod = kod;
  ft->funcs[ft->count].tok = tkn;
  ft->funcs[ft->count].hid  = tkn->hid;
  ft->funcs[ft->count].qytval = 0;  
  ft->funcs[ft->count].hasqyt = 0;
  ft->funcs[ft->count].id  = ft->count;
  ft->funcs[ft->count].ptip = ptip;
  ft->funcs[ft->count].mbtip = mbtip;
  //ft->funcs[ft->count].fat = (struct func_args_table *)malloc(sizeof(struct func_args_table));
  ft->funcs[ft->count].fat.count = 0;
  ft->funcs[ft->count].f_ut.count = 0;
  ft->funcs[ft->count].ql.count = 0;
  ft->funcs[ft->count].tok->sntip = ptip;
  ft->funcs[ft->count].tok->fid = ft->count;

  (ft->count)++;

}

void add_member(struct tok_list_el *tll, struct token *mtok, int mtip, int dec, int ptip, int mbtip){

  struct var_table *mt = &tt->tips[ptip].mt;

  mt->vars[mt->count].tok = mtok;
  mt->vars[mt->count].tip = mtip;
  mt->vars[mt->count].as = dec;
  mt->vars[mt->count].ptip = ptip;
  mt->vars[mt->count].mbtip = mbtip;
  mt->count++;
}



void build_members(struct tok_list_el *tll, int mtip,  int ptip,  int mbtip){

  int i, id = tll->id;
  struct func_table *ft = NULL;
  struct var_table *dt;
  struct tok_list_el *kod;
  struct token *tkn;
  int father;

  if (tll->id == STRUKT2){
    if (tll->childs[0]->id == STRUKT1){
      ptip =  get_tip_id(tll->childs[0]->childs[1]->tok->hid);
	  check_tip(ptip, tll->childs[0]->childs[1]->tok);
      mbtip = 0;
    }
  }

  if (tll->id == FUNK){
    if (tll->childs[1]->cld_cnt == 3)
      kod = tll->childs[1]->childs[1];
    else
      kod = tll->childs[1]->childs[2];
    tkn = tll->childs[0]->childs[0]->childs[1]->tok;
    ft = &tt->tips[ptip].ft;
    dt = &(ft->funcs[ft->count].dt);
	dt->count = 0;
    build_dec_table_yeni(ft, dt, tll , ft->count , &father, 0);
    add_func_table(kod, tkn, ptip, mbtip);
    
    return;
  }

  //we don't have prg1 any more
  if (tll->id == PRG1){

    tkn = tll->childs[0]->tok; /* should be proqram, check!!!  */

    if (tll->cld_cnt == 4){
      override_func_table(tll->childs[2], MAIN_FUNC, glbft, tkn, GLB_TIP, UMUMI);
      return ;      
    }
    /* cld_cnt == 5, see parsing  */
    build_dec_table_yeni(ft, glbmt, tll->childs[2] , 0, &father, 0);
    override_func_table(tll->childs[3], MAIN_FUNC, glbft, tkn, GLB_TIP, UMUMI);
    return;
  }

  if (tll->id == SINIF_TYPE){
    ptip = get_tip_id(tll->childs[0]->childs[0]->childs[1]->tok->hid);
		check_tip(ptip, tll->childs[0]->childs[0]->childs[1]->tok);
  }

  if (tll->id == SINIF_DECL && tll->childs[0]->id != SINIF_DECL)
    mbtip = tll->childs[0]->id;

  if (tll->id == YNT2 && tll->childs[0]->id == YNT1){
       mtip = get_tip_id(tll->childs[0]->childs[0]->tok->hid);
	   check_tip(mtip, tll->childs[0]->childs[0]->tok);
  }

   
 if (tll->id == YNT1){ 
     
	//mtip = get_tip_id(tll->childs[0]->tok->hid);

	switch(tll->childs[1]->id){
	case IDT:
	  add_member(tll->childs[1], tll->childs[1]->tok, mtip, IDT, ptip, mbtip);
	  break;
	case CRG1:
	  add_member(tll->childs[1]->childs[0], tll->childs[1]->childs[0]->tok, mtip, CRG1, ptip, mbtip);
	  break;
	case CRG2:
	  add_member(tll->childs[1]->childs[0], tll->childs[1]->childs[0]->tok, mtip, CRG2, ptip, mbtip);
	  break;
	}

 } 

 
 if (tll->id == YNTS && tll->cld_cnt == 3  ){ 
	switch(tll->childs[2]->id){
	case IDT:
	  add_member(tll->childs[2], tll->childs[2]->tok, mtip, IDT, ptip, mbtip);
	  break;
	case CRG1:
	  add_member(tll->childs[2]->childs[0], tll->childs[2]->childs[0]->tok, mtip, CRG1, ptip, mbtip);
	  break;
	case CRG2:
	  add_member(tll->childs[2]->childs[0], tll->childs[2]->childs[0]->tok, mtip, CRG2, ptip, mbtip);
	  break;
	}
 }


  if (tll->id == YNTS && tll->cld_cnt == 2  ){ 
	switch(tll->childs[1]->id){
	case IDT:
	  add_member(tll->childs[1], tll->childs[1]->tok, mtip, IDT, ptip, mbtip);
	  break;
	case CRG1:
	  add_member(tll->childs[1]->childs[0], tll->childs[1]->childs[0]->tok, mtip, CRG1, ptip, mbtip);
	  break;
	case CRG2:
	  add_member(tll->childs[1]->childs[0], tll->childs[1]->childs[0]->tok, mtip, CRG2, ptip, mbtip);
	  break;
	}
 }


 for (i=0; i<tll->cld_cnt; ++i)
   build_members(tll->childs[i], mtip, ptip, mbtip);
}



void elave_yeni_ynt(struct tok_list_el *tll, struct tok_list_el *ptll){

  strcpy(tt->tips[tt->count].name, tll->childs[1]->tok->tok);
  tt->tips[tt->count].hid = tll->childs[1]->tok->hid;
  tt->tips[tt->count].id  = tt->count;
  //  tt->tips[tt->size].tll  = ptll;
  tt->tips[tt->count].size  = 0;
  //  tt->tips[tt->size].count  = 0;
  //  tt->tips[tt->size].vars  = NULL;
 // tt->tips[tt->count].mt = (struct var_table *)malloc(sizeof(struct var_table));
//  tt->tips[tt->count].ft = (struct func_table *)malloc(sizeof(struct func_table));
  tt->tips[tt->count].tok = tll->childs[1]->tok;
  tt->count++;

}

/* bu funksiyaya tree->first->next -i(STRUKT2) veririrk ki tree -nin lazimsiz budaqlarin  gezmeyek   */
void build_ttable(struct tok_list_el *tll, struct tok_list_el *ptll){
  int i;

  if (tll->id == STRUKT1 && tll->cld_cnt == 3){
    elave_yeni_ynt(tll, ptll);
    return ;
  }

  if (tll->id == SINIF_TYPE){
    elave_yeni_ynt(tll->childs[0]->childs[0], ptll);
    return ;
  }

  for (i=0; i<tll->cld_cnt; ++i)
    build_ttable(tll->childs[i], tll);
}

struct var *get_var(struct var_table *vt, struct usg_list_el *usg){
  int i;

  for (i=0; i<vt->count; ++i)
    if (vt->vars[i].tok->hid == usg->tok->hid)
      return &vt->vars[i];
 
  return NULL;
}

void set_var(struct var *var, struct usg_list_el *usg, int lgm){
  usg->tok->ofst = var->tok->ofst;
  usg->tok->lgm = lgm;
  usg->tok->tip = var->tip;
  usg->tok->size = tt->tips[var->tip].size;
  usg->tok->d1 = var->tok->d1;
  usg->tok->d2 = var->tok->d2;
  usg->tok->mbtip = var->mbtip;
}

void str_memb(struct usg_list_el *usg){

  struct var_table *vt;
  struct var *var;
  struct token *tok;
  int i, tip;

  /* get head tok of parent  */
  tok = head_tok(usg->ptll);

  /* tip should be set   */
  tip = tok->tip;

  vt = &tt->tips[tip].mt;

  if ((var = get_var(vt, usg)) != NULL)
    set_var(var, usg, MEMBER);
  else{
    /* MEMBER of TIP tip NOT FOUND */
	  print_member_not_found_err(usg->tok);
  }
  
}

/* set kod vars (tokens) lgm and offst  */
void build_local_and_member_vars_usg(){

  struct var_table *mt, *dt;
  struct var *var;
  struct usg_table *ut;
  struct usg_list_el *usg;
  struct token *tok;
  int i,j,k;

  for (i=USR_TIP_START; i<tt->count; ++i){

    mt = &tt->tips[i].mt;

    for (j=0; j<tt->tips[i].ft.count; ++j){

      dt = &tt->tips[i].ft.funcs[j].dt;
      ut = &tt->tips[i].ft.funcs[j].ut;

      for (k=0; k<ut->count; ++k){

	usg = &ut->usgs[k];

	if (usg->memb)
	  goto strmemb;

	if ((var = get_var(dt, usg)) != NULL)
	  goto local;

	if ((var = get_var(mt, usg)) != NULL)
	  goto the_member;

	if ((var = get_var(glbmt, usg)) != NULL)
	  goto global;

	goto not_found;

 local:
  //  check_var(var, ut->usg[k]);
  set_var(var, usg, LOCAL);
  continue;

 the_member:
  //  check_var(var, ut->usg[k]);
  set_var(var, usg, MEMBER);
  continue;

 global:
  //  check_var(var, ut->usg[k]);
  set_var(var, usg, GLOBAL);
  continue;

strmemb:
  str_memb(usg);
  continue;

 not_found:
  //print_error
  print_var_not_decld_err(usg->tok);
  continue;
      }
    }
  }      

  return;
}



/* set fid and sntip of funcs  */
void build_local_and_member_funcs_usg(){

  struct func_args_table *fat;
  struct func_table *ft;
  struct func_args_list *fal;
  struct token *tok, *htok;
  int i,j,k;

  for (i=USR_TIP_START; i<tt->count; ++i){

    if ((ft = &tt->tips[i].ft) == NULL)
      continue;

    for (j=0; j<ft->count; ++j){

		if ((ft->funcs[j].fat.count) == 0)
	continue;

		fat = &ft->funcs[j].fat;
      for (k=0; k<fat->count; ++k){

	fal = &fat->fargs[k];
	tok = fal->tok;

	if (fal->memb){
	  htok = head_tok(fal->ptll);
	  get_sntip_and_fid(htok->tip, tok->hid, &tok->sntip, &tok->fid);
	}
	else
	  get_sntip_and_fid(fal->sntip, tok->hid, &tok->sntip, &tok->fid);

	if (tok->fid >= 0)
	  continue;
	  
 not_found:
  //print_error
	print_func_not_found_err(tok);

  continue;
      }
    }
  }      

  return;
}




/* this function sets offst to local and memb vars and
   calculates var_tables sizes and allocates area to hold vars */
void build_vars(struct var_table *vt){

  struct token *tok;
  int i,d1,d2,tipsz;

  vt->size = 0;

  for (i=0; i<vt->count; ++i){
    tok = vt->vars[i].tok;
    d1 = tok->d1;
    d2 = tok->d2;
    tok->ofst = vt->size;
    tipsz = tt->tips[vt->vars[i].tip].size;
    vt->size += tipsz*d1*d2;
  }

  vt->area = (unsigned char *)malloc(vt->size);
}

/*   */
void build_local_and_member_vars_dec(){
  int i,j;

  for (i=USR_TIP_START; i<tt->count; ++i){
    if (tt->tips[i].mt.count != 0)
      build_vars(&tt->tips[i].mt);

    if (tt->tips[i].ft.count > 0)
      for (j=0; j<tt->tips[i].ft.count; ++j)
	build_vars(&tt->tips[i].ft.funcs[j].dt);
  }
}



void add_usg_table(struct tok_list_el *tll, int as, int fid, int member, int parent, struct tok_list_el *ptll, int fa, int sntip){

  struct usg_table *ut = &tt->tips[sntip].ft.funcs[fid].ut;

  ut->usgs[ut->count].as   = as;
  ut->usgs[ut->count].tok  = tll->tok;
  ut->usgs[ut->count].ptll = ptll;  
  ut->usgs[ut->count].fid  = fid;
  ut->usgs[ut->count].memb = member;
  ut->usgs[ut->count].ptip = parent;
  ut->usgs[ut->count].fa   = fa;
  ut->count++;

  /*?  tll->tok->memb = member;?*/

}

int get_func_fid(unsigned long hid, int sntip){

  struct func_table *ft;
  int i;

  ft = &tt->tips[sntip].ft;
  
  if (ft == NULL)
    goto not_found;

  for (i=0; i<ft->count; ++i)
    if (ft->funcs[i].hid == hid)
      return i;

 not_found:
    // function_not_dewclared error;
    return -1;
}


void  get_sntip_and_fid(int sntip, unsigned long hid, int *fsntip, int *fid){

  struct func_table *ft;
  int i;

  ft = &tt->tips[sntip].ft;
  
  if (ft == NULL)
    goto global;

  for (i=0; i<ft->count; ++i)
    if (ft->funcs[i].hid == hid){
      *fsntip = sntip;
      *fid = i;
      return ;
    }

  global:

  ft = &tt->tips[GLB_TIP].ft;
  
  if (ft == NULL)
    goto not_found;

  for (i=0; i<ft->count; ++i)
    if (ft->funcs[i].hid == hid){
      *fsntip = GLB_TIP;
      *fid = i;
	  return ;
    }

 not_found:
  *fid = -1;
  //  printf("Function %ul not declared. ");
    // function_not_declared error;
    return ;
}


/* traverse parse tree and build variable usage table  */
void build_usg_table(struct tok_list_el  *tll, int fid, int member, int ptip, struct tok_list_el *ptll, int fa, int sntip){
  
  int i, tip,  id = tll->id;
  unsigned long hid;

  //    if (tll->id == FCALL1 || tll->id == STRUKT2  )
  if ( tll->id == STRUKT2 || tll->id == FCALL || tll->id == SINIF_HEAD_START || tll->id == YNT2)
    return;

  if (tll->id == SINIF_TYPE) {
    sntip = get_tip_id(tll->childs[0]->childs[0]->childs[1]->tok->hid); 
	check_tip(sntip, tll->childs[0]->childs[0]->childs[1]->tok);
  }

  if (tll->id == FCALL1){
    add_fargs_table(tll, ptll, sntip, fid, member);
    fa = 1;
  }
  
  if (tll->id == IDT && tll->lrb == 0 /* leave  */ ){
    add_usg_table(tll, IDT, fid, member, ptip, ptll, fa, sntip );
    return;
  }

  if (tll->id == STRMEM || tll->id == SNMEM){
    //tip =  get_par_tip(tll->childs[0], member, ptip, fid);
    build_usg_table(tll->childs[0], fid, member, ptip, ptll, fa , sntip);
    //tip for members will be set by build_member_usg
    build_usg_table(tll->childs[2], fid, 1, 0, tll->childs[0], 0, sntip);    
    return;
  }
  
  if (tll->id == CRG1 ){
    add_usg_table(tll->childs[0], CRG1, fid, member, ptip, ptll, fa, sntip);
    build_usg_table(tll->childs[2], fid, 0, 0, NULL, 0, sntip);
    return;
  }

 if (tll->id == CRG2   ){
   add_usg_table(tll->childs[0], CRG2, fid, member, ptip, ptll, fa, sntip);
   build_usg_table(tll->childs[2], fid, 0, 0, NULL, 0, sntip);
   build_usg_table(tll->childs[5], fid, 0, 0, NULL, 0, sntip);
   return;
  }

  if (tll->id == VAR1 || tll->id == FUNK_ELAN)
    return;

  if (tll->id == FUNK){
    hid =  tll->childs[0]->childs[0]->childs[1]->tok->hid;
    fid = get_func_fid(hid, sntip);
	tll->childs[0]->childs[0]->childs[1]->tok->sntip = sntip;
	tll->childs[0]->childs[0]->childs[1]->tok->fid = fid;
  }

  if (tll->id == PRG1){
    // hid =  tll->childs[0]->childs[0]->childs[1]->tok->hid;
    //fid = get_func_fid(hid, sntip);
    fid = MAIN_FUNC;
  }
    
  for (i=0; i<tll->cld_cnt; ++i)
    build_usg_table(tll->childs[i], fid, 0, 0, NULL, fa, sntip);
}

 /* traverse parse tree and build function usg table  */
void build_func_usg_table(struct tok_list_el  *tll, struct tok_list_el *ptll){
  
  struct token *tok;
  struct f_usgs *f_ut;
  int i;

  if (tll->id == SNMEM && ptll->id == KODDATA)
	  return;

  if (tll->id == FCALL1 && ptll->id != KODDATA){
	  tok = tll->childs[0]->childs[0]->tok;
	  f_ut = &tt->tips[tok->sntip].ft.funcs[tok->fid].f_ut;
	  f_ut->usgs[f_ut->count].tok = tok;
	  f_ut->usgs[f_ut->count].qyt_val = 1;
	  f_ut->count++;
  }
  

  for (i=0; i<tll->cld_cnt; ++i)
    build_func_usg_table(tll->childs[i],tll);
}

void add_qaytar_el(struct tok_list_el  *tll,int sntip, int fid){

	struct qyt_list *ql;

	ql = &tt->tips[sntip].ft.funcs[fid].ql;
	ql->qyts[ql->count].tok = tll->childs[0]->tok;

	if (tll->cld_cnt == 3)
		ql->qyts[ql->count].qyt_res = 1;
	else
		ql->qyts[ql->count].qyt_res = 0;

  ql->count++;
}

 /* traverse parse tree and build function return table  */
void build_func_qyt_list(struct tok_list_el  *tll,int sntip, int fid){
  
  struct token *tok;
  int i;

  if (tll->id == FUNK){
	  tok = tll->childs[0]->childs[0]->childs[1]->tok;
	  sntip = tok->sntip;
	  fid = tok->fid;
  }

  if (tll->id == QAYTAR1){
	add_qaytar_el(tll, sntip, fid);
	return;
  }

  for (i=0; i<tll->cld_cnt; ++i)
    build_func_qyt_list(tll->childs[i], sntip, fid);
}

/* set name for main func  */
void create_main_func_token(struct token *tkn){

  unsigned long h;

	tkn->tok = (char *)malloc(10);
	strcpy(tkn->tok, "proqram");
	hash((unsigned char *)tkn->tok, &h);
	tkn->hid = h;

}

void build_symtab(){

	struct token *tkn = (struct token *)malloc(sizeof(struct token));

  build_ttable(tree->first->next, tree->first->next);
  build_members(tree->first->next, 0, GLB_TIP, UMUMI);
  create_main_func_token(tkn);
  override_func_table(tree->first->next, MAIN_FUNC, glbft, tkn, GLB_TIP, UMUMI);
  init_tips_size(tt);
  build_local_and_member_vars_dec(); /* setting ofst and size for dec  */
  build_usg_table(tree->first->next, 0, 0, 0, NULL, 0, GLB_TIP);
  build_local_and_member_vars_usg();  /*  setting ofst and size for usg  */
  build_local_and_member_funcs_usg(); /*  set sntip and fid of funcs  */
  build_func_usg_table(tree->first->next, tree->first->next); //functions that resturns value
  build_func_qyt_list(tree->first->next, 0, 0 ); //build funcs return list

  return; 
  /*

  build_func_table(func_tab, &dec_tab, tree->first->next, 0, SERBEST); 
  build_func_start_end(&dec_tab, func_tab);

  build_vars(&dec_tab, &usg_tab);
  build_fargs_table(fat,tree->first->next,0,0);
  */
}

void check_double_declared_types(struct ttable *ttab){

	struct token *toki, *tokj;
	int i,j; 

  for (i=0; i<ttab->count - 1; ++i)
	  if (strcmp(ttab->tips[i].name, "\0")!=0)
		for (j = i+1; j < ttab->count ; ++j)
			if (strcmp(ttab->tips[i].name, ttab->tips[j].name)==0){
				toki = ttab->tips[i].tok;
				tokj = ttab->tips[j].tok;
				if (strcmp(ttab->tips[i].name, "tam")==0      ||\
					strcmp(ttab->tips[i].name, "kesr")==0     ||\
					strcmp(ttab->tips[i].name, "simvol")==0   ||\
					strcmp(ttab->tips[i].name, "GLBPRG\0")==0 )

					print_std_type_decl_err(tokj, ttab->tips[i].name );
					
				else					
					print_type_dbl_dec_err(tokj, toki, ttab->tips[i].name);
					
				break;
			}



}

void check_double_declared_Var_Members(struct ttable *ttab){

	struct var_table *mt;
	struct token *tokj, *tokk;
	int i,j,k;

	for (i=0; i<ttab->count; ++i){
		if (strcmp(ttab->tips[i].name, "GLBPRG") == 0 || strcmp(ttab->tips[i].name, "\0")==0)
			continue; /* proqramin eyniadli heddlerini bashcur yoxlamaliyiq   */
		mt = &ttab->tips[i].mt;
		for (j = 0; j<mt->count - 1; ++j )	
			for (k = j+1; k<mt->count; ++k )	
				if (mt->vars[j].tok->hid == mt->vars[k].tok->hid){
					tokj = mt->vars[j].tok;
					tokk = mt->vars[k].tok;
					//report error
					print_dbl_dec_memb_err(tokk, tokj, ttab->tips[i].name);
					break;
				}
	
	}

}

//before we must check double declared func names

void check_double_declared_Var_Local(struct ttable *ttab){

	struct var_table *vt;
	struct token *tokk, *tokm;
	int i,j,k,m;

	for (i=0; i<ttab->count; ++i){
		 if (strcmp(ttab->tips[i].name, "\0")==0)
			 continue;
		 for (j=0; j<ttab->tips[i].ft.count; ++j){
			vt = &ttab->tips[i].ft.funcs[j].dt;
			for (k=0; k < vt->count - 1; k++)
				for (m = k + 1; m < vt->count; ++m)
					if (vt->vars[k].tok->hid == vt->vars[m].tok->hid){
						tokk = vt->vars[k].tok;
						tokm = vt->vars[m].tok;
						print_dbl_dec_var_loc_err(tokm, tokk, ttab->tips[i].ft.funcs[j].tok->tok );
						break;
				}
	
		 }
	}

}

void check_double_declared_Var_Global(struct ttable *ttab){

	struct var_table *mt;
	struct token *tokj, *tokk;
	int i,j,k;

	for (i=0; i<ttab->count; ++i)
		if (strcmp(ttab->tips[i].name, "GLBPRG") == 0 )
			break;
			
	mt = &ttab->tips[i].mt;

	for (j = 0; j<mt->count - 1; ++j )	
		for (k = j+1; k<mt->count; ++k )	
			if (mt->vars[j].tok->hid == mt->vars[k].tok->hid){
				tokj = mt->vars[j].tok;
				tokk = mt->vars[k].tok;
				print_dbl_dec_var_glb_err(tokk, tokj, tokk->tok);
				break;
			}
}


void  check_double_declared_Funcs(struct ttable *ttab){

	struct func_table *ft;
	struct token *tokj, *tokk;
	int i,j,k,m;

	for (i=0; i<ttab->count; ++i){
		 if (strcmp(ttab->tips[i].name, "\0")==0)
			 continue;
		 ft = &ttab->tips[i].ft;
		 for (j=0; j < ft->count - 1; ++j){
			 for (k= j + 1; k < ft->count; k++)
				 if (ft->funcs[j].tok->hid == ft->funcs[k].tok->hid){
						tokj = ft->funcs[j].tok;
						tokk = ft->funcs[k].tok;
						print_dbl_dec_func_err(tokk, tokj, tokj->tok);
						break;
				}	
		 }
	}

}


/* we are checking only count  */
void check_func_Args(struct ttable *ttab){

 struct token *fstok, *fdtok;
 int i,j,k, sntip, fid;

 for (i = 0; i < ttab->count; ++i){
	if (strcmp(ttab->tips[i].name, "\0")==0)
		continue;
	for (j=0; j<ttab->tips[i].ft.count; ++j)
		for (k=0; k < ttab->tips[i].ft.funcs[j].fat.count; ++k){
			sntip = ttab->tips[i].ft.funcs[j].fat.fargs[k].tok->sntip;
			fid	  = ttab->tips[i].ft.funcs[j].fat.fargs[k].tok->fid;
			if (sntip < 0 || fid < 0)
				continue;
			if (ttab->tips[sntip].ft.funcs[fid].pl.count < ttab->tips[i].ft.funcs[j].fat.fargs[k].count){
				fstok = ttab->tips[sntip].ft.funcs[fid].tok;
				fdtok = ttab->tips[i].ft.funcs[j].fat.fargs[k].tok;
				print_func_more_parms_err(fdtok, fstok, fdtok->tok);
			}
			else
				if (ttab->tips[sntip].ft.funcs[fid].pl.count > ttab->tips[i].ft.funcs[j].fat.fargs[k].count){
					fstok = ttab->tips[sntip].ft.funcs[fid].tok;
					fdtok = ttab->tips[i].ft.funcs[j].fat.fargs[k].tok;
					print_func_less_parms_err(fdtok, fstok, fdtok->tok);
				}
		}
 }

}


/* we are checking only count  */
void check_f_usg_vs_qyt(struct ttable *ttab){

 struct token *ftok;

 struct f_usgs *fl;
 struct qyt_list *ql;

 int i,j,k, sntip, fid, func_rets_val, f_usg_requires_val_row,f_usg_requires_val_fid ;

 for (i = 0; i < ttab->count; ++i){

	if (strcmp(ttab->tips[i].name, "\0")==0)
		continue;

	for (j=0; j<ttab->tips[i].ft.count; ++j){
	
		fl = &ttab->tips[i].ft.funcs[j].f_ut;
		ql = &ttab->tips[i].ft.funcs[j].ql;
		ftok = ttab->tips[i].ft.funcs[j].tok;

		func_rets_val = 0;

		for (k=0; k<fl->count; ++k)
			if (fl->usgs[k].qyt_val){
				func_rets_val = 1;
				f_usg_requires_val_row = fl->usgs[k].tok->row;
				f_usg_requires_val_fid = k;
				break;
			}

		for (k=0; k<ql->count; ++k){

			if (ql->qyts[k].qyt_res && func_rets_val)
				continue;

			/* cancelled for the moment 
			if (ql->qyts[k].qyt_res && !func_rets_val){
				printf("xeberdarliq(%d,%d): %s funksiyasinin netice qaytarmasi hec bir yerde teleb olunmur.\n",ql->qyts[k].tok->row, ql->qyts[k].tok->col, ftok->tok );
				wrn_cnt++;
				continue;
			}
			*/

			if (!ql->qyts[k].qyt_res && !func_rets_val)
				continue;

			if (!ql->qyts[k].qyt_res && func_rets_val){
				print_func_must_ret_val_err(ql->qyts[k].tok, fl->usgs[0].tok->tok, f_usg_requires_val_row);
			}

			}

		if (ql->count == 0 && func_rets_val){
			print_func_must_ret_val_err(ftok, ftok->tok, f_usg_requires_val_row);
		}

		}

	}

}

void check_symtab(){



  check_double_declared_types(tt);
  check_double_declared_Funcs(tt);
  check_double_declared_Var_Global(tt);
  check_double_declared_Var_Members(tt);
  check_double_declared_Var_Local(tt);
  check_func_Args(tt);
  check_f_usg_vs_qyt(tt);

  check_symtab_errors();

  /*
  check_double_declared_funcs(func_tab);
  check_confl_func_vars(func_tab, &dec_tab);
  check_variable_usage_yeni( &usg_tab, &dec_tab, func_tab);
  check_member_usage( &usg_tab, &dec_tab, func_tab);
  check_str_mems(ytab);
  check_fargs(func_tab, fat);
  check_brk_cnt(tree->first->next, 0,0);
  check_sec_all(tree->first->next);
  */



		//skip for the moment
		//exit(1);

}

void print_symtab(){
//   print_dec_table(&dec_tab);
  //print_usg_table(&usg_tab);
  //  print_func_table(func_tab);
     print_ttable(tt);
  //  print_fargs_table(fat);
}


void symtab(){
 
  reset_error_counter();

  init_symtab();
  build_symtab();
  check_symtab();
  //print_symtab();
}



