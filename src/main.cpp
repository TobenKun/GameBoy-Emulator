#include <iostream>
#include "cpu/CPU.h"

int main(int ac, char** av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <ROM>\n";
		std::exit(1);
	}

	CPU cpu;

	cpu.LoadROM(av[1]);
	return (0);
}
