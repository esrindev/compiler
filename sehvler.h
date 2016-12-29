
/* ahmed.sadikhov@gmail.com
   here we define all our LIMITS
*/

#ifndef SEHVLER_H
#define SEHVLER_H


 extern void 	print_func_must_ret_val_err(struct token *tok, char *name, int row);
 extern void print_func_less_parms_err(struct token *fdtok, struct token *fstok, char *name);
  /* print expected string without surrounding ``  */
extern void print_before(struct tok_list_el *tll, char *msg);
extern void print_after(struct tok_list_el *tll, char *msg);
extern void print_type_not_def_err(struct token *tok);
extern void print_var_dbl_dec_err(struct token *tokk, struct token *tokj, char *name);
extern void print_member_not_found_err(struct token *tok);
extern void print_var_not_decld_err(struct token *tok);
extern void print_func_not_found_err(struct token *tok);
extern void print_std_type_decl_err(struct token *tok, char *name );
extern void print_type_dbl_dec_err(struct token *tokj, struct token *toki, char *name);
extern void print_dbl_dec_var_loc_err(struct token *tokm, struct token *tokk, char *name);
extern void print_dbl_dec_var_glb_err(struct token *tokk, struct token *tokj, char *name);
extern void print_dbl_dec_func_err(struct token *tokk, struct token *tokj, char *name);
extern void print_func_more_parms_err(struct token *fdtok, struct token *fstok, char *name);
extern void print_dbl_dec_memb_err(struct token *tokk, struct token *tokj, char *name);

extern void print_token_type_1_err(int row, int col, char *msg);
extern void print_token_type_2_err(int row, int col, char *msg, char c);
extern void print_token_type_3_err(int row, int col, char *msg, char *s);


#endif
