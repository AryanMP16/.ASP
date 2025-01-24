#ifndef EMULATOR_H
#define EMULATOR_H

enum reg {
  rax, //return register
  rsp, //stack pointer
  rip, //instruction pointer
  r1,
  r2,
  r3,
  r4,
  r5,
  r6,
  r7,
  r8,
  r9,
  r10,
  r11,
  r12
};

int memory[4096] = {0}; //address ranges from 0x1000 to 0x2000
int access_memory(int address) {
  //starting memory accessing from 0x1000, so "address" 0x1000 needs to map to index 0
  return memory[address - 4096];
}

#endif
