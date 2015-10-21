 
 #include  "esrin.h"

void dump_to_file(char *fname, struct band_list_el band[], int band_size){

  int el_size, count, size_t;

  el_size =  sizeof(struct band_list_el);
 
  FILE *f = fopen(fname, "wb");
  size_t = fwrite((struct band_list_el*)band, band_size, el_size, f);
  if (ferror (f))
	  printf ("Ikili fayla yazma xetasi prg.bin\n");
  fclose(f);

  /*
  printf("%d elements written to band.\n", band_size);
  printf("sizeof one lement is %d.\n", el_size);
  //printf("file size is %d\n", );
  printf("number of bytes written to file %d\n",size_t);
  */


/*
  FILE *f = fopen(name, "rb");
  fseek(f,0,SEEK_END);
  size = ftell(f);
  fseek(f,0,SEEK_SET);

  text = (char *)malloc(size+1);
  memset(text, '\0', size+1);

  fread(text, size, 1, f);
  fclose(f);
*/

}

void insert_glbmt(struct band_list_el *band){

  band[0].dt_size = tt->tips[GLB_TIP].mt.size;
  band[0].id = GLBMT;
  band[0].band_id = GLBMT;
  strcpy(band[0].tok,"glb_tip");
}

void generate(){

  /* build execution band  */
  insert_glbmt(exec_band); /* only size needed  */
  band_size = 1;
  build_exec_band(tree->first->next, exec_band, &band_size);  
  print_exec_band(exec_band, band_size);
  init_funcs_ret_addrs(exec_band, band_size);
  
  dump_to_file("prg.esr_bin", exec_band, band_size);
}
