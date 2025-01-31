#ifndef EMULATOR_H
#define EMULATOR_H

//Memory allocation:
//0x0000 - 0x000F: instructions, numbered 0x0 - 0xF
//0x0010 - 0x001F: registers
//0x1000 - 0x2000: writable memory

//Only allow each entry of the array to be at most 0xFF. C will obviously allow any value, but that's not how we want our memory emulator to work.

extern int memory[0xFFFF]; //emulated memory
extern int rax, rsp, rip, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12; //emulated regiter file

void write_byte(int address, int to_write);
int read_byte(int address);
void initialize_emulator();
void display_memory(int start_address, int num_bytes);
void read_asp_file(char* filename);
void execute_program(); //This is the bulk of the emulator.

typedef struct operand {
  int type;
  int value;
} operand;

#endif
