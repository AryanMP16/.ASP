#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#define NUM_INSTRUCTIONS 2

typedef struct inst{
  int m_opcode;
  char* m_name;
  int m_num_operands;
  char** operands;
} inst;

extern inst* instructions;
void create_instructions();
void lexer(char* filename);

#endif
