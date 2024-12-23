#include "cpu/CPU.h"

int main(int ac, char** av)
{
	CPU cpu;

	cpu.LoadROM(av[1]);
	return (0);
}
