#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int memory[0xFFFF] = {0};

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
  printf("\n");
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
