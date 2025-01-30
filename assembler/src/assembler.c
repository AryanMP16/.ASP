#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 10000
#define NUM_INSTRUCTIONS 15

int asp_file_contents_length = -1;
int num_tokens = 0;
inst* instructions = NULL;
char* asp_file_contents = NULL;
void missing_start();
int is_delimeter(char i);
int get_type(char* str);
int encode_operand(token operand);

void create_instructions() {
  instructions = (inst*)malloc(sizeof(inst) * NUM_INSTRUCTIONS);

  //In general, OP Src, Dest
  //interaction with memory
  inst LW = {0x01, "LW", 2, {INTEGER_LITERAL, REGISTER}};
  inst SW = {0x02, "SW", 2, {REGISTER, INTEGER_LITERAL}};
  inst LOI = {0x03, "LOI", 2, {IMMEDIATE, REGISTER}};

  //arithmetic
  inst ADD = {0x04, "ADD", 2, {REGISTER, REGISTER}};
  inst SUB = {0x05, "SUB", 2, {REGISTER, REGISTER}};
  inst LSH = {0x06, "LSH", 2, {INTEGER_LITERAL, REGISTER}};
  inst RSH = {0x07, "RSH", 2, {INTEGER_LITERAL, REGISTER}}; //arithmetic shift

  //control flow
  inst JMP = {0x08, "JMP", 1, {INTEGER_LITERAL}};
  inst JNZ = {0x09, "JNZ", 1, {INTEGER_LITERAL}};
  inst CMP = {0x0A, "CMP", 2, {REGISTER, REGISTER}};

  //logic
  inst AND = {0x0B, "AND", 2, {REGISTER, REGISTER}};
  inst OR = {0x0C, "OR", 2, {REGISTER, REGISTER}};
  inst XOR = {0x0D, "XOR", 2, {REGISTER, REGISTER}};
  inst NOT = {0x0E, "NOT", 1, {REGISTER}};
  
  inst TER = {0x0F, "TER", 0, {-1, -1}};

  instructions[0] = LW;
  instructions[1] = SW;
  instructions[2] = LOI;
  instructions[3] = ADD;
  instructions[4] = SUB;
  instructions[5] = LSH;
  instructions[6] = RSH;
  instructions[7] = JMP;
  instructions[8] = JNZ;
  instructions[9] = CMP;
  instructions[10] = AND;
  instructions[11] = OR;
  instructions[12] = XOR;
  instructions[13] = NOT;
  instructions[14] = TER;
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
  printf("Illegal token: '%s'\nAllowed tokens are of type:\n\tINSTRUCTION (no prefix)\n\tIMMEDIATE (prefix '$'. MUST be in hex with prefix '0x')\n\tREGISTER (prefix '%%')\n\tINTEGER_LITERAL. MUST be in hex with prefix '0x'\n", str);
  exit(1);
}

int is_delimeter(char i){
  return (i == ' ' || i == ',' || i == '\n');
}

inst token_to_inst(token t) {
  for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
    if (!strcmp(t.str, instructions[i].m_name)) {
      return instructions[i];
    }
  }
  printf("Parsing error\n");
  exit(1);
}

void parser(token* token_stream, int debug_mode) {
  int stream_traverser = 0;
  int num_expressions = 0;

  FILE* f;
  f = fopen("assembled", "w");
  
  while (stream_traverser < num_tokens) {
    if (token_stream[stream_traverser].type == INSTRUCTION) {
      inst this_instruction = token_to_inst(token_stream[stream_traverser]);
      debug_mode ? printf("Instruction: %s || Number of operands: %d", this_instruction.m_name, this_instruction.m_num_operands) : (void) 0;

      debug_mode ? printf(" || Opcode: %d", this_instruction.m_opcode) : (void) 0;
      fprintf(f, "0x%02x ", this_instruction.m_opcode);
      
      if (this_instruction.m_num_operands != 0) {
	debug_mode ? printf("\n\tOperands: ") : (void) 0;
	for (int i = 0; i < this_instruction.m_num_operands; i++) {
	  debug_mode ? printf("%s ", interpret_type(this_instruction.operands[i])) : (void) 0;
	  token this_token = token_stream[stream_traverser + i + 1];
	  
	  if (this_token.type == this_instruction.operands[i]) {
	    //This is the "things are going right" branch
	    debug_mode ? printf("Y ") : (void) 0;
	    debug_mode ? printf("Encoding: %d || ", encode_operand(this_token)) : (void) 0;
	    
	    fprintf(f, "0x%02x ", this_token.type);
	    //little-endianness
	    for (int i = 0; i < 3; i++) {
	      fprintf(f, "0x%02x ", ((unsigned int) encode_operand(this_token) >> 8 * i) & 0x00000FF); //unsigned so that shift is always logical
	    }
	    
	  }
	  else {
	    debug_mode ? printf("X ") : (void) 0;
	    printf("\n### Error: incorrect operand type or count at line %d ###\n", num_expressions + 2);
	    exit(1);
	  }
	}
	debug_mode ? printf("\n") : (void) 0;
      }
      else {
	debug_mode ? printf("\n") : (void) 0;
      }
      stream_traverser += this_instruction.m_num_operands + 1;
      num_expressions++;
    }
    else { //should only ever read instructions, and handle operands in loop above.
      printf("### Error: incorrect operand count at line %d ###\n", num_expressions + 1);
      exit(1);
    }
    fprintf(f, "\n");
  }
  debug_mode ? printf("Total expressions read: %d\n", num_expressions) : (void) 0;
  fclose(f);
}

char* interpret_type(int type) {
  char* to_return = (char*)malloc(sizeof(char) * 100);
  switch (type) {
  case INSTRUCTION:
    strcpy(to_return, "INSTRUCTION");
    break;
  case IMMEDIATE:
    strcpy(to_return, "IMMEDIATE");
    break;
  case REGISTER:
    strcpy(to_return, "REGISTER");
    break;
  case INTEGER_LITERAL:
    strcpy(to_return, "INTEGER LITERAL");
    break;
  }
  return to_return;
}

int encode_operand(token operand) {
  int to_return = 0;
  switch (operand.type) {
  case REGISTER:
    char str[4];
    strncpy(str, operand.str + 1, 3);
    if (!strcmp(str, "rax"))
      to_return = rax;
    else if (!strcmp(str, "rsi"))
      to_return = rsp;
    else if (!strcmp(str, "rip"))
      to_return = rip;
    else if (!strcmp(str, "r1"))
      to_return = r1;
    else if (!strcmp(str, "r2"))
      to_return = r2;
    else if (!strcmp(str, "r3"))
      to_return = r3;
    else if (!strcmp(str, "r4"))
      to_return = r4;
    else if (!strcmp(str, "r5"))
      to_return = r5;
    else if (!strcmp(str, "r6"))
      to_return = r6;
    else if (!strcmp(str, "r7"))
      to_return = r7;
    else if (!strcmp(str, "r8"))
      to_return = r8;
    else if (!strcmp(str, "r9"))
      to_return = r9;
    else if (!strcmp(str, "r10"))
      to_return = r10;
    else if (!strcmp(str, "r11"))
      to_return = r11;
    else if (!strcmp(str, "r12"))
      to_return = r12;
    else {
      printf("\nIncorrect register: %s\n", operand.str);
      exit(1);
    }
    break;
  case IMMEDIATE:
    char* imm = operand.str + 1;
    to_return = strtol(imm, NULL, 16);
    break;
  case INTEGER_LITERAL:
    char* intl = operand.str;
    to_return = strtol(intl, NULL, 16);
    break;
  }
  return to_return;
}
