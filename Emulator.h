#pragma once
class Emulator
{
public:
	Emulator();
	virtual ~Emulator();
	bool screen[32][64];

	void LoadInMemory(short int addr, unsigned char* buffer, short int size);
	void Step(short int PressedKey);
private:
	unsigned char RAM[4096];
	unsigned char reg[16];
	short int stack[24];
	short int stacklevel;
	short int I;
	short int PC;
	short int Delay_Timer;
	short int Sound_Timer;
	double lastclock;
	double clockspeed = 1 / 60;
	bool Halt;
};

