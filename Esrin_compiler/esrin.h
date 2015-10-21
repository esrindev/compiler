#ifndef ESRIN_H
#define ESRIN_H


  
#include <windows.h>
#include <conio.h>
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "defs.h"
#include "states.h"


#define  MAXSIZE 1048576  /* 1 MB */

struct variable;
struct func_table_el;

struct token {
  /* identification  */
  char *tok;
  int id;
  int prs_id;
  unsigned long  hid;
  /* variable recognition  */
  int tip; 
  int size; 
  int ofst; /* id in vartab, ofst of parent el in case of fcall  */
  int lgm;  /* local, global or member  */

  /* function recognition  */
  int fid; 
  int sntip;

  /* fcall  */
  int fargsid;

  /* for structs */
  int ptip;  
  int memb; 
  int mbtip;

  /* dimensions for array   */
  int d1;
  int d2;

  /* location  */
  int row;
  int col;
};


#define MAX_PARS 1024

struct se{
  int start, end;
};

struct node {
  struct token *parsed[MAX_PARS];
  int pct;
  struct se se;
  int ind;
  int id;
};

#define MAX_NOD 1048576

/* lists definition   */

struct tok_list_el {
  struct token *tok; /* col and row staff  */
  int id; /* parsing */
  struct tok_list_el  *next;
  struct tok_list_el  *prev;
  struct tok_list_el  *childs[20]; /* max 20 child allowed, hope that's enough  */
  int cld_cnt; /* number of childs of this childb   */
  int old_copy; /* indicates whether this node is copied from lower layer
                      directly , or by consyming transformed into new node  
		 possible bug! we set this filed to 1 on copy_to_upper_layer
          function, may that be set otherwise on layer constucting functions?
    if so, then we lose control over the parse tree. For the moment I just skip verification for late  */
  int cons_id; /* why consuming   */

  int lrb; /*  leave or branch: 0 - leave, 1 - branch
               all tokens initialized to leave  */
  int fargsid; /* id ind func args tab if it is fcall1   */  
  /* exec band  */
  int head_pos;
  int band_id;
}; 

struct tok_list{
  struct tok_list_el *first;
  struct tok_list_el *last;
  int count;
};

struct layer{
  struct tok_list tok_list;
  int lay_num;
  struct layer *next;
  struct layer *prev;
};

struct parse_tree{
  struct layer  *top;
  struct layer  *bottom;
  int cnt;
};

/* old, should be removed 
struct variable {
  int id;
  struct token *tok;
  int tip;
  int dec_pos;
  int use_pos;
  int i_val;
  double d_val;
  char c_val;
  int decl_as;
  int usg_cnt;
};
*/

/* variable   */
struct var{
  struct tok_list_el *tll;
  struct token *tok;
  unsigned long fhid;
  int fid;
  int sntip; /* to define function, added after sinif  */
  //moved to token's lgm  int snmb; /* var is sinif member, not local var  */
  int tip;
  int ptip;
  int as; /* variables can be declared as IDT, CRG1 or CRG2 */
  int memb;
  /* those used for classes */
  int mbtip; /* private, public, protected  */
  int fa; /* if used as function argument  */
};


struct var_table{
  struct var vars[MAX_DEC];
  unsigned char *area; /* area to stare variables  */
  int size;
  int count;
};


struct dec_list_el{
  struct token *tok;
  unsigned long fhid;
  int fid;
  int sntip; /* to define function, added after sinif  */
  int dec_tip;
  int dec_as;
  /* those used for classes */
  int mbtip; /* private, public, protected  */
};

struct dec_table{
  struct var decs[MAX_DEC];
  int size;
};


struct usg_list_el{
  struct token *tok;
  struct tok_list_el *ptll;
  int fid;
  int as;
  int memb;
  int ptip;
  int fa; /* if used as function argument  */
};

struct usg_table{
  struct usg_list_el usgs[MAX_USG];
  int count;
};

#if 0
struct pre_list_el{
  int tip; 
  int id; 
  struct pre_list_el *next;
  struct tok_list_el *tll;
};

struct pre_list{
  struct pre_list_el *first;
  struct pre_list_el *last;
  int pre_cnt;
};

#endif

struct int_variables{
  int *vars;
  int size;
};


struct char_variables{
  char *vars;
  int size;
};


struct float_variables{
  long double *vars;
  int size;
};

/* max numb of func dec params  */
#define MAX_PAR 100

/*
struct par_list_el{
  int dec_tip;
  int usg;
};
*/

struct par_list{
  struct var *pars[MAX_PAR];
  int count;
};

#define MAX_FUNC_ARGS 100

struct func_arg_el{
  struct tok_list_el *tll;
  int usg;
  int tip; /* future use */
  long double val; /* used for passing value to func  */
};

struct func_args_list{
  struct func_arg_el fels[MAX_FUNC_ARGS];
  struct tok_list_el *tll; /* link to fcall1 tll  */
  struct tok_list_el *ptll; /* link to parent of fcall1 in case of object member*/
  struct token *tok; /* funk tok  */
  int count;
  int memb;
  int sntip;
  int psntip;
  int fid;
};


#define MAX_FUNC_CALLS 100 /* max numb of func call declarations
, not actual calls  */

struct func_args_table{
  struct func_args_list fargs[MAX_FUNC_CALLS];
  int count;
};


struct func_table_el{
  struct token *tok; /* token hilding name of function in declaration  */
  unsigned long hid;
  int id;
  struct tok_list_el *kod;
  int start; /* start and end of func variable pos in dec_tab  */
  int end;
  long double qytval;
  int hasqyt;
  /* used for sinif  */ 
  /* important note!!! all functions defined out of sinif has:
  sntip = 0, mbtip = SERBEST  */
  int sntip; /*depricated  */
  int ptip;    /* parent's tip*/
  int mbtip;   /* mexusi, serbest or qorunan  */
  struct var_table dt; /* local vars  */
  struct par_list  pl; /* par list  */
  struct usg_table ut; 
  unsigned char *varea; /*reference to storeage of local vars of this func  */
  struct func_args_table fat;
  int head_pos; /* address of func kod on exec band  */
};

struct func_table{
  struct func_table_el funcs[MAX_FUNC];
  int count;
};


extern struct func_args_table fat_cont, *fat;

struct stek_el{
  void *dptr;
  struct stek_el *prev;
};

struct stek{
  struct stek_el *top;
};

struct member{
  int tip;
  int dec;
  struct token *tok;
  int ind; /* optional  */
  struct tok_list_el *tll;
};

/* tip, user defined or standard  */
struct tip{
  int id; /* ind in ttab  */
  unsigned long hid;
  char name[MAX_NAME_LEN];
  int size;   /* size of this tip  */
  unsigned char  *marea; /* start address of meber variables of this tip  */
  struct var_table  mt; /* member table  */
  struct func_table ft; /* function table  */
};

/* table to hold standard and user defined types   */
struct ttable{
  struct tip  tips[MAX_TIP];
  int count;
};

/* used for function argument comparision  */
struct cmp_prm{
  /* if passed argument is variable get inf about its declaration var dec, vd , othervise NULL  */
  struct dec_list_el *vd;
  /* fa - function argument, what passed to function at call  */
  int fa_tip;
  int fa_usg;
  int fa_row;
  int fa_d1;
  int fa_d2;
  char *fa_nm;
  /* fp - function parameter , what declared   */
  struct dec_list_el *fp;
};


struct band_list_el {
  char tok[MAX_TOK_LEN];
  int id;
  int band_id;
  int d1;
  int d2;
  int row;
  int col;
  int lgm;
  int ofst;
  int tip;
  int size;
  int fid;
  int sntip;
  int head_pos;
  unsigned char *dt_area;
  int dt_size;
};



extern struct stek fnstk, obstk, dovrstk, secstk, glbstk, valstk, adrstk;

extern struct ytable ytable_cont, *ytab;

extern int psh;

extern struct int_variables   int_variables_cont;
extern struct int_variables   *int_vars;
extern struct char_variables  char_variables_cont;
extern struct char_variables  *char_vars;
extern struct float_variables float_variables_cont;
extern struct float_variables *float_vars;

#define MAX_VAR 100000

extern struct node *nodes[MAX_NOD];
extern int ndc;

extern char *output;

extern FILE *fp;

extern struct token tokens[];
extern int tks;

/* parse tree  */
extern struct tok_list *tree;

extern int err_cnt;

extern struct dec_table dec_tab;
extern struct usg_table usg_tab;

extern struct func_table func_table_cont, *func_tab;
extern struct func_table snfunc_table_cont, *snfunc_tab;

extern int  next_tok();
extern int las_tid();
extern int las_tok;

extern int parse();

extern void translate_to_cpp(void);

extern void print_one_node(struct node *nd);

extern struct tok_list_el *get_first_child(struct tok_list_el *tll);

extern void print_tree_dx(struct tok_list *tl);


extern void add_dec_table(struct func_table *ft, struct var_table *dt, struct tok_list_el *tll, int tip, int as, int fid, int father);

extern void print_dec_table(struct var_table *dec_tab);
extern void print_usg_table(struct usg_table *usg_tab);
extern void build_dec_table_yeni(struct func_table *ft, struct var_table *dt, struct tok_list_el  *tll , int fid, int *father, int tip);

extern void build_dec_table(struct func_table *ft, struct dec_table *dt, struct tok_list_el  *tll, int fid, int father);

extern void init_hash_ids();
extern void print_hash();

extern int check_double_declaration(struct dec_table *dt);
extern int check_variable_usage( struct usg_table *ut, struct dec_table *dt, struct func_table *ft);
extern int check_members_usage( struct usg_table *ut, struct dec_table *dt, struct func_table *ft);

extern void build_list_capet(struct tok_list_el *tll, struct tok_list *lst);

extern void print_tokens_with_id();
extern void print_tok(int tok);
extern void print_tok_str(int tok);

extern char *file_name;


extern long double get_val(struct tok_list_el *tll);

extern long double eval_single_el(struct tok_list_el *tll);

extern long double eval_pre_list(struct tok_list lst);

extern void build_func_table(struct func_table *ft, struct dec_table *dt, struct tok_list_el  *tll, int sntip, int mbtip );
extern void build_snfunc_table(struct func_table *ft, struct dec_table *dt, struct tok_list_el  *tll , int sntip, int mbtip);

extern int check_double_declared_funcs(struct func_table *ft);
extern int check_confl_func_vars(struct func_table *ft, struct dec_table *dt);
extern struct tok_list_el *get_funk_kod(struct func_table *ft, unsigned long hid);

extern void build_func_start_end(struct dec_table *dt, struct func_table *ft);

extern void print_vars(struct usg_table *ut);

extern void   print_func_parms_list(struct tok_list vl);
extern long double eval_single_el_cond(struct int_variables *int_vars, struct tok_list_el *tll);

extern int match_inacsfE(int k);
extern int get_tip(unsigned long hid);

extern int get_decsi(unsigned long hid);

extern void init_func_args_table(struct func_args_table *fat_cont, struct func_args_table **fat_ref);

extern struct dec_list_el *get_var_dec(unsigned long hid, int fid);

extern void init_ytable(struct ytable *ytab_cnt, struct ytable **ytab);
extern void build_ytable(struct tok_list_el *ynt_node, struct tok_list_el *ptll);
extern void print_ytable(struct ytable *ytab);

extern void check_str_mems(struct ytable *ytab);
unsigned char *get_address_new(struct tok_list_el *tll, int *tip);
extern long double get_value(unsigned char *adrs, int tip, int size);
extern void set_value(unsigned char * dst, unsigned char *src, int size);
extern void get_address_and_tip(struct tok_list_el *tll, unsigned char **address, int *tip);

extern struct dec_list_el *get_var_dec_glb(unsigned long hid, int fid);

extern void sehv_1(struct usg_list_el *ull);
extern void sehv_2(struct dec_list_el *dll, struct usg_list_el *ull);
extern void sehv_3(struct dec_list_el *dll, struct usg_list_el *ull);
extern void sehv_4(struct dec_list_el *dll, struct usg_list_el *ull);
extern void sehv_5(struct dec_list_el *dll, struct usg_list_el *ull);
extern void sehv_6(struct dec_list_el *dll, struct usg_list_el *ull);
extern void sehv_7(struct dec_list_el *vll, struct dec_list_el *pll, char *fname, int row,  int ind);
extern void sehv_8(struct dec_list_el *vll, struct dec_list_el *pll, char *fname, int row,  int ind);
extern void sehv_9(struct dec_list_el *pll, char *fname, int row,  int ind);
extern void sehv_10(struct dec_list_el *pll, char *fname, int row,  int ind);


extern void print_tip(int tip);

extern struct dec_list_el *init_vd(struct token *tok, int id, int pfid);

struct token *head_tok(struct tok_list_el *tll);

extern struct tok_list_el* get_member_fll(int ptip, struct tok_list_el *cld);
extern void tokenize(char *);
extern void symtab();
extern void execute ();

extern void init_stek();

extern struct func_table *glbft;
extern struct var_table *glbmt;
extern struct ttable ttable_cont, *tt;

extern void check_errors(struct tok_list *tree);

extern struct tok_list_el *create_tok_list_el(struct token *tok);

extern struct band_list_el  exec_band[BAND_SIZE] ;
extern int band_size, prg_start;

extern void generate();
extern void init_bottom_layer_new(struct tok_list *tree);
void hash(unsigned char *str, unsigned long *h);
extern void build_exec_band(struct tok_list_el *tll, struct band_list_el band[], int *size);
extern void init_funcs_ret_addrs(struct band_list_el band[], int size);
extern void print_exec_band(struct band_list_el band[], int size);

extern void pop_from_stek_win(struct stek *stk, void **ptr);

#endif

