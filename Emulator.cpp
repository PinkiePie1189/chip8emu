#include "stdafx.h"
#include "Emulator.h"
#include <stdlib.h>
#include <time.h>

Emulator::Emulator()
{
	memset(RAM, 0, sizeof(RAM));
	memset(reg, 0, sizeof(reg));
	stacklevel = -1;
	I = 0x200;
	PC = 0x200;
	Delay_Timer = 0;
	Sound_Timer = 0;
	Halt = false;
	srand(time(NULL));
}

void Emulator::LoadInMemory(short int addr, unsigned char* buffer, short int size)
{
	memcpy(RAM + addr, buffer, size);
}

void Emulator::Step(short int PressedKey) {
	short int Code = RAM[PC] / 0x10;
	short int X = RAM[PC] % 0x10;
	short int Y = RAM[PC + 1] / 0x10;
	short int N = RAM[PC + 1] % 0x10;

	printf("%d\n", PressedKey);
	if (Code == 0x00 && RAM[PC + 1] == 0xE0) {
		printf_s("Clearing Screen...\n");
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 64; x++) {
				screen[y][x] = 0;
			}
		}
	} else if (Code == 0x00 && RAM[PC + 1] == 0xEE) {
		PC = stack[stacklevel] - 2;
		stacklevel--;
	} else if (Code == 0x1) {
		short int addr = X * 0x100 + Y * 0x10 + N;
		PC = addr - 2;
	} else if (Code == 0x2) {
		short int addr = X * 0x100 + Y * 0x10 + N;
		stack[++stacklevel] = PC + 2;
		PC = addr - 2;
	} else if (Code == 0x3) {
		if (reg[X] == RAM[PC + 1])
			PC += 2;
	} else if (Code == 0x4) {
		if (reg[X] != RAM[PC + 1])
			PC += 2;
	} else if (Code == 0x5) {
		if (reg[X] == reg[Y])
			PC += 2;
	} else if (Code == 0x6) {
		reg[X] = RAM[PC + 1];
	} else if (Code == 0x7) {
		short int res = reg[X] + RAM[PC + 1];
		res &= 0xFF;
		reg[X] = res;
	} else if (Code == 0x8 and N == 0) {
		reg[X] = reg[Y];
	} else if (Code == 0x8 and N == 1) {
		reg[X] |= reg[Y];
	} else if (Code == 0x8 and N == 2) {
		reg[X] &= reg[Y];
	} else if (Code == 0x8 and N == 3) {
		reg[X] ^= reg[Y];
	} else if (Code == 0x8 and N == 4) {
		short int res = reg[X] + reg[Y];
		if (res > 0xFF) {
			reg[0xF] = 1;
		} else {
			reg[0xF] = 0;
		}
		res &= 0xFF;
		reg[X] = res;
	} else if (Code == 0x8 and N == 5) {
		short int res = reg[X] - reg[Y];
		if (res < 0) {
			reg[0xF] = 0;
			res += 0xFF;
		}
		else {
			reg[0xF] = 1;
		}
		reg[X] = res;
	} else if (Code == 0x8 and N == 6) {
		reg[0xF] = reg[X] & 1;
		reg[X] >>= 1;
	} else if (Code == 0x8 and N == 7) {
		short int res = reg[Y] - reg[X];
		if (res < 0) {
			reg[0xF] = 0;
			res += 0xFF;
		}
		else {
			reg[0xF] = 1;
		}
		reg[X] = res;
	} else if (Code == 0x8 and N == 0xE) {
		if (reg[X] & 128)
			reg[0xF] = 1;
		else
			reg[0xF] = 0;

		short int res = reg[X] << 1;
		res &= 0xFF;
		reg[X] = res;
	} else if (Code == 0x9) {
		if (reg[X] != reg[Y])
			PC += 2;
	} else if (Code == 0xA) {
		short int addr = X * 0x100 + Y * 0x10 + N;
		I = addr;
	} else if (Code == 0xB) {
		short int addr = X * 0x100 + Y * 0x10 + N;
		PC = addr + reg[0] - 2;
	} else if (Code == 0xC) {
		reg[X] = rand() & 0xFF;
	} else if (Code == 0xD) {
		for (int line = 0; line < N; line++) {
			unsigned char CurrentByte = RAM[I + line];
			reg[0xF] = 0;

			for (int coordX = reg[X]; coordX < reg[X] + 8; coordX++) {
				if (coordX >= 64 || reg[Y] + line >= 32)
					continue;
				bool bit = CurrentByte & 128;
				CurrentByte <<= 1;

				if (bit) {
					if (screen[reg[Y] + line][coordX] == 1) {
						reg[0xF] = 1;
						screen[reg[Y] + line][coordX] = 0;
					} else {
						screen[reg[Y] + line][coordX] = 1;
					}
				}
			}
		}
	} else if (Code == 0xE && RAM[PC + 1] == 0x9E) {
		if (PressedKey == reg[0])
			PC += 2;
	} else if (Code == 0xE && RAM[PC + 1] == 0xA1) {
		if (PressedKey != reg[0])
			PC += 2;
	} else if (Code == 0xF && RAM[PC + 1] == 0x07) {
		reg[X] = Delay_Timer;
	} else if (Code == 0xF && RAM[PC + 1] == 0x0A) {
		if (PressedKey == -1) {
			Halt = true;
		} else {
			reg[X] = PressedKey;
			Halt = false;
		}
	} else if (Code == 0xF && RAM[PC + 1] == 0x15) {
		Delay_Timer = reg[X];
	} else if (Code == 0xF && RAM[PC + 1] == 0x18) {
		Sound_Timer = reg[X];
	} else if (Code == 0xF && RAM[PC + 1] == 0x1E) {
		I += reg[X];
	} else if (Code == 0xF && RAM[PC + 1] == 0x29) {
		printf_s("Tried to load character: %x\n", reg[X]);
	} else if (Code == 0xF && RAM[PC + 1] == 0x33) {
		RAM[I] = reg[X] / 100;
		RAM[I + 1] = (reg[X] / 10) % 10;
		RAM[I + 2] = reg[X] % 10;
	} else if (Code == 0xF && RAM[PC + 1] == 0x55) {
		memcpy(RAM + I, reg, X + 1);
	} else if (Code == 0xF && RAM[PC + 1] == 0x65) {
		memcpy(reg, RAM + I, X + 1);
	} else {
		printf_s("Undefined OPCode: %x%x\n", RAM[PC], RAM[PC + 1]);
	}
	
	double currentclock = clock() / (double) CLOCKS_PER_SEC;
	if (currentclock - lastclock > clockspeed) {
		if (Delay_Timer > 0)
			Delay_Timer--;
		if (Sound_Timer > 0)
			Sound_Timer--;

		lastclock = currentclock;
	}

	if (Halt == false)
		PC += 2;
}

Emulator::~Emulator()
{
}
