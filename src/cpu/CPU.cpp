#include "CPU.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

CPU::CPU() : AF({0}), BC({0}), DE({0}), HL({0}), sp(0), pc(0)
{
	memory.fill(0);
	// else
}

CPU::~CPU()
{
	// do something
}

bool CPU::LoadROM(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		if (size > 0x8000)
		{
			std::cerr << "ROM size exceeds maximum addressable space (32KB)."
					  << std::endl;
			return (false);
		}
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> romData(size);
		if (!file.read(reinterpret_cast<char*>(romData.data()), size))
		{
			std::cerr << "Failed to read ROM file." << std::endl;
			return (false);
		}
		for (size_t i = 0; i < romData.size(); i++)
		{
			memory[i] = romData[i];
		}
		std::cout << "ROM loaded successfully: " << filename << std::endl;
		return (true);
	}
	std::cerr << "Failed to open ROM file: " << filename << std::endl;
	return (false);
}

void CPU::PrintRegisters() const
{
	printf("AF: %04X BC: %04X DE: %04X HL: %04X SP: %04X PC: %04X\n", AF.value,
		   BC.value, DE.value, HL.value, sp, pc);
}

void CPU::PrintFlags() const
{
	printf("Z: %01X N: %01X H: %01X C: %01X unused: %04X\n", F.z, F.n, F.h, F.c,
		   F.unused);
}
