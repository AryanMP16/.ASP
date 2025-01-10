#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSTRUCTION_ARRAY_PATH "/mnt/c/Users/aryan/OneDrive/Desktop/ISA_project/src/instruction_array.txt"
#define MAX_BUFFER_SIZE 1000000
#define MAX_NUM_INSTRUCTIONS 100

//read instruction_array.txt and process instruction information
ISA* create_ISA() {
  ISA to_ret = (ISA*)malloc(sizeof(ISA));
  to_ret->m_instructions = (inst*)malloc(sizeof(inst) * MAX_NUM_INSTRUCTIONS);
  to_ret->num_instructions = 0;

  //open aforementioned file
  FILE* f = fopen(INSTRUCTION_ARRAY_PATH, "r");
  if (f == NULL) {
    printf("Could not find ISA file!\n");
    exit(1);
  }

  //read file into buffer
  char buffer[MAX_BUFFER_SIZE + 1];
  int terminator_index = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, f);
  buffer[terminator_index] = '\0';

  int reset_line = 0; //false
  int opcode_set = 0;
  int name_set = 0;
  int num_operands_set = 0;
  int operands_set = 0;
  
  for (int i = 0; i < terminator_index; i++) {
    inst current_instruction = {-1, "", -1, ""};
    char* opcode_string = "";
    char* num_operands_string = "";
    
    if (!reset_line) {
      if (buffer[i] != ',' && buffer[i] != '\n') {
	if (!opcode_set)
	  strcat(opcode_string, &buffer[i], 1);
	else if (!name_set)
	  strcat(current_instruction.name, &buffer[i], 1);
	else if (!num_operands_set)
	  strcat(num_operands_string, &buffer[i], 1);
	else if (!operands_set)
	  //Create internal for loop, process the operands, set mark @ newline
	  int a = 1;
      }
      else if (buffer[i] == ',') {
	if (!opcode_set)
	  opcode_set = 1;
	else if (!name_set)
	  name_set = 1;
	else if (!num_operands_set)
	  num_operands_set = 1;
	else if (!operands_set)
	  operands_set = 1;
      }
      else if (buffer[i] == '\n') {
	reset_line = 1;
      }
    }
    else {
      opcode_set = 0;
      name_set = 0;
      num_operands_set = 0;
      operands_set = 0;
    }

    current_instruction.m_opcode = atoi(opcode_string);
    current_instruction.m_num_operands = atoi(num_operands_string);

    to_ret->num_instructions++;
    //add current_instruction to m_instructions array
  }
  return to_ret;
}
