#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Did not specify a file to assemble!\n");
    exit(1);
  }
  lexer(argv[1]);
  
  return 0;
}
