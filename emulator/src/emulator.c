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
  int ZERO_FLAG = -1;
  
  while(1) {
    int instruction = read_byte(rip);
    //we can take advantage of the fact that instructions must have at most 2 operands, each of which will be precisely 9 bytes wide. Instructions that have 1 operand (or 0) will still fill the other operand fields; they'll just fill them with 0s.
    operand operand1 = {read_byte(rip + 1), read_byte(rip + 2) + read_byte(rip + 3) * 0x100 + read_byte(rip + 4) * 0x10000};
    operand operand2 = {read_byte(rip + 5), read_byte(rip + 6) + read_byte(rip + 7) * 0x100 + read_byte(rip + 8) * 0x10000};    
    
    //All other code is general, but the below switch case is specific to my ISA.
    //Implementation of my specific assembly program
    //printf("Instruction: 0x%02x. Op1: 0x%x. Op2: 0x%x\n", instruction, operand1.value, operand2.value);
    
    switch (instruction) {
    case 0x01: //LW
      *_register(operand2.value) = read_byte(operand1.value);
      rip += 0x9;
      break;
    case 0x02: //SW
      write_byte(operand2.value, *_register(operand1.value));
      rip += 0x9;
      break;
    case 0x03: //LOI
      *_register(operand2.value) = operand1.value;
      rip += 0x9;
      break;
    case 0x04: //MOVR
      *_register(operand2.value) = *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x05: //ADD
      *_register(operand2.value) += *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x06: //ADDI
      *_register(operand2.value) += operand1.value;
      rip += 0x9;
      break;
    case 0x07: //SUB
      *_register(operand2.value) -= *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x08: //LSH
      *_register(operand2.value) = *_register(operand2.value) << operand1.value;
      rip += 0x9;
      break;
    case 0x09: //JMP
      rip = operand1.value;
      break;
    case 0x0A: //JZ
      rip = ZERO_FLAG ? operand1.value : rip + 0x9;
      break;
    case 0x0B: //CMP
      ZERO_FLAG = *_register(operand2.value) - *_register(operand1.value) == 0 ? 1 : 0;
      rip += 0x9;
      break;
    case 0x0C: //AND
      *_register(operand2.value) &= *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x0D: //OR
      *_register(operand2.value) |= *_register(operand1.value);
      rip += 0x9;
      break;
    case 0x0E: //NOT
      *_register(operand2.value) = !(*_register(operand1.value));
      rip += 0x9;
      break;
    case 0x0F: //TER
      printf("Program run successfully. Value in %%rax is 0x%x (hexadecimal)/%d (decimal)\n", rax, rax);
      exit(0);
      break;
    }
  }
}

void write_byte(int address, int to_write) {
  if (address < 0x1000) {
    printf("Segmentation fault.\n");
    exit(1);
  }
  if (to_write > 0xFF) {
    printf("Tried to write byte larger than 0xFF: 0x%x\n", to_write);
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
