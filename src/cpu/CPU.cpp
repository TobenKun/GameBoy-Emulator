#include "CPU.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

CPU::CPU()
{
	std::memset(memory, 0, sizeof(memory));
	// else
}

CPU::~CPU() {}

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
