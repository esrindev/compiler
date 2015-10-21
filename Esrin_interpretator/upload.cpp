 
 #include  "esrin.h"

int get_size(char *fname){

    FILE * fd=fopen(fname, "rb");

    if(!fd)
        printf("ikili fayl acilmir %s\n", fname);

    fseek(fd, 0, SEEK_END);
    size_t size = ftell(fd);
    fclose(fd);
    return size;

}

void save_el_bin(struct band_list_el *bll, struct band_list_el_bin *bin_bll){
/*
  bll->id = bll->id;
  band[*size].band_id = id;
  strcpy(band[*size].tok, tok->tok);
  band[*size].d1 = tok->d1;
  band[*size].d2 = tok->d2;
  band[*size].row = tok->row;
  band[*size].col = tok->col;
  band[*size].tip = tok->tip;
  band[*size].size = tok->size;
  band[*size].lgm = tok->lgm;
  band[*size].ofst = tok->ofst;
  band[*size].fid = tok->fid;
  band[*size].sntip = tok->sntip;
  band[*size].head_pos = hpos;
*/
   
}

	//struct band_list_el_bin band_bin[BAND_SIZE];

void load_bin(char *fname, struct band_list_el band[], int *band_size){

  int size, el_size, count, size_t;

  el_size =  sizeof(struct band_list_el);
  FILE *f = fopen(fname, "rb");
  fseek(f,0,SEEK_END);
  size = ftell(f);
  fseek(f,0,SEEK_SET);
  count = size / el_size;
  size_t = fread((struct band_list_el*)band, count, el_size, f);

  if (ferror (f))
	  printf ("Ikili fayldan oxuna bilmir prg.bin\n");
  fclose(f);
  *band_size = count;

  /*
  printf("%d elements read to band.\n", count);
  printf("size_t  = %d.\n", size_t);
  printf("sizeof one lement is %d.\n", el_size);
  */
 
}



