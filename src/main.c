#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help() {
  printf("Command: asp\nOptions:\n    --help\tyou've just used this\t\tno file input required\n    -d\t\trun assembler in debug mode\tneed input file\n");
}

int main(int argc, char* argv[]) {
  int exit_code = 0;
  if (argc < 2) {
    printf("Did not specify a file to assemble!\n");
    exit_code = 1;
  }
  else if (argc == 2) {
    if (!strcmp(argv[1], "--help")) {
      help();
    }
    else if (argv[1][0] == '-') {
      printf("Option not recognized!\n");
      help();
    }
    else {
      create_instructions();
      readfile(argv[1]);
      token* token_array = lexer();
      parser(token_array);
    }
  }
  else {
    if (!strcmp(argv[2], "--help"))
      help();
    else if (!strcmp(argv[2], "-d")) {
      printf("Interpreting ISA...");
      create_instructions();
      printf("success.\n");
      readfile(argv[1]);
      printf("File read successful.\nFor debugging purposes, here is the asp file:\n%s\n...Running lexer...\n", asp_file_contents);
      token* token_array = lexer();
      printf("Number of tokens read: %d\n", num_tokens);
      for (int i = 0; i < num_tokens; i++)
	printf("Token: '%s', type: %s\n", token_array[i].str, interpret_type(token_array[i].type));
      printf("Lexer ran with no errors\n...Running parser...\n");
      parser(token_array);
    }
    else {
      printf("Option not recognized!\n");
      help();
      exit_code = 1;
    }
  } 
  return exit_code;
}
