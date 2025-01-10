#ifndef EMULATOR_H
#define EMULATOR_H

typedef struct inst{
  int m_opcode;
  char* m_name;
  int m_num_operands;
  char** operands;
} inst;

typedef struct ISA{
  inst* m_instructions;
  int num_instructions;
} ISA;

ISA* create_ISA(inst* instructions);

#endif
