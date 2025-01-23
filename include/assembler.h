#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#define NUM_INSTRUCTIONS 2

enum type {
  INSTRUCTION, IMMEDIATE, REGISTER, ENDLINE
};

typedef struct inst {
  int m_opcode;
  char* m_name;
  int m_num_operands;
  char** operands;
} inst;

typedef struct expression {
  inst instruction;
  int* operands; //registers represented as ENUMs, mem addresses are already integers
  //to prevent register ENUM from clashing with memory, mem addresses will start at
  //0x1000
} expression;

typedef struct token {
  char* str;
  int length;
  int type;
} token;

extern char* asp_file_contents;
extern inst* instructions;

void create_instructions();
void readfile(char* filename);
token* lexer(); //must be called AFTER readfile

#endif
