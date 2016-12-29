/* GPL  V.3  
  Esrin Compiler Main Module
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
*/

#include "esrin.h"

int main(int argc, char *argv[]){

  printf("Proqram kompilyasiya olunur...\n");
 
  tokenize("prg.esr");

  parse();

  symtab();

  printf("Kod generasiya olunur...\n");

  generate();

  printf("Tamam.\n");

}
 
