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
	// cpu.run();

	return (0);
}

// void register_test()
// {
// 	cpu.F.z = 0;
// 	cpu.F.n = 0;
// 	cpu.F.h = 0;
// 	cpu.F.c = 1;
// 	std::cout << (bool)(cpu.AF.value & (1 << 7));
// 	std::cout << (bool)(cpu.AF.value & (1 << 6));
// 	std::cout << (bool)(cpu.AF.value & (1 << 5));
// 	std::cout << (bool)(cpu.AF.value & (1 << 4));
// 	std::cout << (bool)(cpu.AF.value & (1 << 3));
// 	std::cout << (bool)(cpu.AF.value & (1 << 2));
// 	std::cout << (bool)(cpu.AF.value & (1 << 1));
// 	std::cout << (bool)(cpu.AF.value & (1 << 0));
// 	std::cout << std::endl;
//
// 	cpu.AF.value = 0xABCD;
// 	cpu.PrintFlags();
// 	cpu.PrintRegisters();
//
// 	cpu.BC.value = 0x1234;
// 	std::cout << std::hex << (int)cpu.BC.hi << std::endl;
// 	std::cout << std::hex << (int)cpu.BC.lo << std::endl;
//
// 	uint32_t num = 1;
// 	uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
//
// 	if (ptr[0] == 1)
// 	{
// 		std::cout << "Little Endian" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Big Endian" << std::endl;
// 	}
// }
