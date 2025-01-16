#include "assembler.h"
#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 10000

inst* instructions = NULL;
void missing_start();

void create_instructions() {
  instructions = (inst*)malloc(sizeof(inst) * NUM_INSTRUCTIONS);

  char* LOI_operands[2] = {"imm", "reg"};
  inst LOI = {0x01, "LOI", 2, LOI_operands};
  
  char* ADD_operands[2] = {"reg", "reg"};
  inst ADD = {0x02, "ADD", 2, ADD_operands};

  char* SUB_operands[2] = {"reg", "reg"};
  inst SUB = {0x03, "SUB", 2, SUB_operands};

  inst TER = {0xFF, "TER", 0, NULL};

  instructions[0] = LOI;
  instructions[1] = ADD;
  instructions[2] = SUB;
  instructions[3] = TER;
}

expression* lexer(char* filename) {
  //check file validity
  int i = 0;
  while (filename[i] != '\0')
    i++;

  if (i <= 4 || !(filename[i - 1] == 'p' &&
		  filename[i - 2] == 's' &&
		  filename[i - 3] == 'a' &&
		  filename[i - 4] == '.')) {
    printf("Incorrect file name! Make sure your file ends in '.asp'!\n");
    exit(1);
  }
  
  //open file
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    printf("Could not find specified file!\n");
    exit(1);
  }
  
  //read file into buffer
  char buffer[MAX_BUFFER_SIZE + 1];
  int terminator_index = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, f);
  buffer[terminator_index] = '\0';

  //check for start
  if (strncmp(buffer, ">start", 6) != 0)
    missing_start();

  //count number of lines (number of expressions)
  int num_exp_in_file = 0;
  for (int j = 0; j < terminator_index; j++) {
    if (buffer[j] == '\n')
      num_exp_in_file++;
  }
  
  //lexer algorithm
  expression* expression_array = (expression*)malloc(sizeof(expression) * num_exp_in_file);
  //TODO
  

  //return expression array
  return expression_array;
}

void missing_start() {
  printf("Start of file not found! Ensure the first line of your assembly is '>start'!\n");
  exit(1);
}
