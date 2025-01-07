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

void CPU::run()
{
	while (1)
	{
		cycle();
	}
}

void CPU::cycle()
{
	opcode = memory[pc++];
	switch (opcode)
	{
		// nop
		case 0x00:
			opcode_nope();
			break;

		// ld r16, imm16
		case 0x01:
		case 0x11:
		case 0x21:
		case 0x31:
			opcode_LD_r16_imm16();
			break;

		// ld [r16mem], a
		case 0x02:
		case 0x12:
		case 0x22:
		case 0x32:
			opcode_LD_r16mem_A();
			break;

		// ld a, [r16mem]
		case 0x0A:
		case 0x1A:
		case 0x2A:
		case 0x3A:
			opcode_LD_A_r16mem();
			break;

		// ld [imm16], sp
		case 0x08:
			opcode_LD_imm16_sp();
			break;

		// inc r16
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
			opcode_INC_r16();
			break;

		// dec r16
		case 0x0B:
		case 0x1B:
		case 0x2B:
		case 0x3B:
			opcode_DEC_r16();
			break;

		// add hl, r16
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
			opcode_ADD_hl_r16();
			break;

		// add inc r8
		case 0x04:
		case 0x0C:
		case 0x14:
		case 0x1C:
		case 0x24:
		case 0x2C:
		case 0x34:
		case 0x3C:
			opcode_INC_r8();
			break;

		// dec r8
		case 0x05:
		case 0x0D:
		case 0x15:
		case 0x1D:
		case 0x25:
		case 0x2D:
		case 0x35:
		case 0x3D:
			opcode_DEC_r8();
			break;

		default:
			opcode_dummy();
			break;
	}
}

// FOR TEST!!
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
