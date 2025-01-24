#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>

enum type {
  INSTRUCTION, IMMEDIATE, REGISTER, INTEGER_LITERAL
};

typedef struct inst {
  int m_opcode;
  char* m_name;
  int m_num_operands;
  int operands[2]; //maximum number of operands is 2
} inst;

typedef struct expression {
  inst instruction;
  int* exp_operands; //registers represented as ENUMs, mem addresses are already integers
  //to prevent register ENUM from clashing with memory, mem addresses will start at
  //0x1000
} expression;

typedef struct token {
  char* str;
  int type;
} token;

extern char* asp_file_contents;
extern inst* instructions;
extern int num_tokens;

void create_instructions();
void readfile(char* filename);
token* lexer(); //must be called AFTER readfile
void parser(token* token_stream);
char* interpret_type(int type);

#endif
