#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#define NUM_INSTRUCTIONS 2

//register names inspired by x86
enum reg;

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

extern inst* instructions;
void create_instructions();
expression* lexer(char* filename);

#endif
