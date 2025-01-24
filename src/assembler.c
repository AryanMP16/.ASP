#include "assembler.h"
#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 10000
#define NUM_INSTRUCTIONS 4

int asp_file_contents_length = -1;
int num_tokens = 0;
inst* instructions = NULL;
char* asp_file_contents = NULL;
void missing_start();
int is_delimeter(char i);
int get_type(char* str);

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
  asp_file_contents = buffer;
}

void missing_start() {
  printf("Start of file not found! Ensure the first line of your assembly is '>start'!\n");
  exit(1);
}

token* lexer() {
  int array_size = 1000; //start with 1000 tokens
  token* to_return = (token*)malloc(sizeof(token) * array_size);
  int r_traverser = 7; int l_traverser = 7; int was_prev_delim = 0;

  while (l_traverser < asp_file_contents_length && r_traverser < asp_file_contents_length && l_traverser <= r_traverser) {
    if (!is_delimeter(asp_file_contents[r_traverser])) {
      r_traverser++;
      was_prev_delim = 0;
    }
    else {
      if (!was_prev_delim){
	char* str = (char*) malloc(r_traverser - l_traverser + 1);
	strncpy(str, asp_file_contents + l_traverser, r_traverser - l_traverser);
	str[r_traverser - l_traverser] = '\0';

	int type = get_type(str);
	token this_token = {malloc(r_traverser - l_traverser + 1), type}; //have to do it this way because mutilation of string literals is undefined behaviour
	strcpy(this_token.str, str);
	if (num_tokens < array_size)
	  to_return[num_tokens] = this_token;
	else {
	  array_size *= 2;
	  to_return = (token*)realloc(to_return, sizeof(token) * array_size);
	  to_return[num_tokens] = this_token;
	}
	num_tokens++;

	r_traverser++;
	l_traverser = r_traverser;
	was_prev_delim = 1;
      }
      else { //double (or more) delimeter
	r_traverser++;
	l_traverser = r_traverser;
	was_prev_delim = 1;
      }
    }
  }
  
  return to_return;
}

int get_type(char* str) {
  if (instructions == NULL) {
    printf("Instructions not initialized!\n");
    exit(1);
  }
  for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
    if (!strcmp(str, instructions[i].m_name))
      return INSTRUCTION;
  }
  if (str[0] == '$')
    return IMMEDIATE;
  if (str[0] == '%')
    return REGISTER;
  if (str[0] == '0' && (str[1] == 'x' || str['b']))
    return INTEGER_LITERAL;
  printf("Illegal token: '%s'\nAllowed tokens are of type:\n\tINSTRUCTION (no prefix)\n\tIMMEDIATE (prefix '$')\n\tREGISTER (prefix '%')\n\tINTEGER_LITERAL (prefix '0x' for hexadecimal or '0b' for binary)\n", str);
  exit(1);
}

int is_delimeter(char i){
  return (i == ' ' || i == ',' || i == '\n');
}
