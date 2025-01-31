#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int memory[0xFFFF] = {0};
int rax, rsp, rip, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;

int* _register(int rcode);

void initialize_emulator() {
  for (int i = 0x00; i < 0x10; i++) {
    memory[i] = i;
  }
}

void display_memory(int start_address, int num_bytes) {
  for (int i = 0; i < num_bytes; i++)
    printf("0x%02x\t", memory[start_address + i]);
  printf("\n");
}

void read_asp_file(char* filename) {
  size_t MAX_BUFFER_SIZE = 1000000;
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    printf("Could not find specified file!\n");
    exit(1);
  }

  char buffer[MAX_BUFFER_SIZE + 1];
  int terminator_index = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, f);
  buffer[terminator_index] = '\0';

  int j = 0;
  int i = 0;
  while (j < terminator_index) {
    if (buffer[j] == '\n')
      memmove(&buffer[j], &buffer[j + 1], strlen(buffer) - j);
    //Every character that is at index 5i, 5i+1, 5i+4 is just aesthetics
    if (j % 5 != 0 && (j - 1) % 5 != 0 && (j - 4) % 5 != 0) {
      char* byte_str = (char*)malloc(sizeof(char) * 2);
      byte_str[0] = buffer[j]; byte_str[1] = buffer[j + 1];
      unsigned int num = (unsigned int)strtol(byte_str, NULL, 16);
      write_byte(0x1000 + i, num);
      i++;
      j++;
    }
    j++;
  }
}

void execute_program() {
  rip = 0x1000; //this is where instructions start from
  //incrementing of rip is done by individual functions, not by this function

  printf("Starting program. Here is the register file before any instructions are executed:\nrax: 0x%04x\trsp: 0x%04x\trip: 0x%04x\t\nr1: 0x%03x\tr2: 0x%03x\tr3: 0x%03x\tr4: 0x%03x\tr5: 0x%03x\tr6: 0x%03x ...\n\n", rax, rsp, rip, r1, r2, r3, r4, r5, r6);
  
  for(int i = 0; i < 5; i++) {
    int instruction = read_byte(rip);
    //we can take advantage of the fact that instructions must have at most 2 operands, each of which will be precisely 9 bytes wide. Instructions that have 1 operand (or 0) will still fill the other operand fields; they'll just fill them with 0s.
    operand operand1 = {read_byte(rip + 1), read_byte(rip + 2) + read_byte(rip + 3) * 0x100 + read_byte(rip + 4) * 0x10000};
    operand operand2 = {read_byte(rip + 5), read_byte(rip + 6) + read_byte(rip + 7) * 0x100 + read_byte(rip + 8) * 0x10000};    
    
    printf("Instruction executing: 0x%02x; Operand 1: 0x%03x; Operand 2: 0x%03x\n", instruction, operand1.value, operand2.value);

    //All other code is general, but the below switch case is specific to my ISA.
    //Implementation of my specific assembly program
    switch (instruction) {
    case 0x03:
      *_register(operand2.value) = operand1.value;
      rip += 0x9;
      break;
    case 0x04:
      *_register(operand2.value) += *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x0F:
      printf("Program run successfully. Value in %%rax is 0x%04x\n", rax);
      exit(0);
      break;
    }

    printf("Instruction executed. Here is the register file afterwards:\nrax: 0x%04x\trsp: 0x%04x\trip: 0x%04x\t\nr1: 0x%03x\tr2: 0x%03x\tr3: 0x%03x\tr4: 0x%03x\tr5: 0x%03x\tr6: 0x%03x ...\n\n", rax, rsp, rip, r1, r2, r3, r4, r5, r6);
  }
}

void write_byte(int address, int to_write) {
  if (address < 0x1000) {
    printf("Segmentation fault.\n");
    exit(1);
  }
  if (to_write > 0xFF) {
    printf("Tried to write byte larger than 0xFF\n");
    exit(1);
  }
  memory[address] = to_write;
}

int read_byte(int address) {
  if (address > 0xFFFF) {
    printf("How much memory do you need?! You tried to write past 65 KiB (memory address 0xFFFF)");
    exit(1);
  }
  if (address < 0x1000) {
    printf("Segmentation fault.\n");
    exit(1);
  }
  return memory[address];
}

int* _register(int rcode)  {
  switch (rcode) {
  case 0:
    return &rax;
    break;
  case 1:
    return &rsp;
    break;
  case 2:
    return &rip;
    break;
  case 3:
    return &r1;
    break;
  case 4:
    return &r2;
    break;
  case 5:
    return &r3;
    break;
  case 6:
    return &r4;
    break;
  case 7:
    return &r5;
    break;
  case 8:
    return &r6;
    break;
  case 9:
    return &r7;
    break;
  case 10:
    return &r8;
    break;
  case 11:
    return &r9;
    break;
  case 12:
    return &r10;
    break;
  case 13:
    return &r11;
    break;
  case 14:
    return &r12;
    break;
  }
}
