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

enum reg {
  rax, //return register
  rsp, //stack pointer
  rip, //instruction pointer
  r1, r2, r3,
  r4, r5, r6,
  r7, r8, r9,
  r10, r11, r12
};

extern char* asp_file_contents;
extern inst* instructions;
extern int num_tokens;

void create_instructions();
void readfile(char* filename);
token* lexer(); //must be called AFTER readfile
void parser(token* token_stream, int debug_mode);
char* interpret_type(int type);

#endif
