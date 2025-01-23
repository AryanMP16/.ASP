#include "assembler.h"
#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 10000

int asp_file_contents_length = -1;
inst* instructions = NULL;
char* asp_file_contents = NULL;
void missing_start();
int is_delimeter(char i);

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

void readfile(char* filename) {
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
  asp_file_contents_length = terminator_index;

  //check for start
  if (strncmp(buffer, ">start", 6) != 0)
    missing_start();

  printf("File print successful. No read errors\n");
  asp_file_contents = buffer;
}

void missing_start() {
  printf("Start of file not found! Ensure the first line of your assembly is '>start'!\n");
  exit(1);
}

token* lexer() {
  token* to_return_temporary = (token*)malloc(sizeof(token*));

  int r_traverser = 7; int l_traverser = 7; int was_prev_delim = 0;
  while (l_traverser < asp_file_contents_length && r_traverser < asp_file_contents_length && l_traverser <= r_traverser) {
    if (!is_delimeter(asp_file_contents[r_traverser])) {
      r_traverser++;
      was_prev_delim = 0;
    }
    else if (is_delimeter(asp_file_contents[r_traverser] && l_traverser < r_traverser)) {
      if (!was_prev_delim /*ignoring double delimeter for now*/){
	char* str = (char*) malloc(r_traverser - l_traverser + 1);
	strncpy(str, asp_file_contents + l_traverser, r_traverser - l_traverser);
	str[r_traverser - l_traverser] = '\0';
	printf("%s\n", str);
      }
    }
  }
  
  printf("Lexer ran with no errors\n");
  return to_return_temporary;
}

int is_delimeter(char i){
  return (i == ' ' || i == ',');
}
