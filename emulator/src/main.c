#include "emulator.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  initialize_emulator();
  read_asp_file(argv[1]);
  display_memory(0x1000, 128);
  execute_program();
  
  return 0;
}
