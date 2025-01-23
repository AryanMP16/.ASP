#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Did not specify a file to assemble!\n");
    exit(1);
  }
  readfile(argv[1]);
  printf("For debugging purposes, here is the asp file:\n%s\n...Running lexer...\n", asp_file_contents);
  lexer();
  
  return 0;
}
