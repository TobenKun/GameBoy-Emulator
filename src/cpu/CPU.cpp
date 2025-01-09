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
		if (!stopped) cycle();
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
			opcode_ld_r16_imm16();
			break;

		// ld [r16mem], a
		case 0x02:
		case 0x12:
		case 0x22:
		case 0x32:
			opcode_ld_r16mem_a();
			break;

		// ld a, [r16mem]
		case 0x0A:
		case 0x1A:
		case 0x2A:
		case 0x3A:
			opcode_ld_a_r16mem();
			break;

		// ld [imm16], sp
		case 0x08:
			opcode_ld_imm16_sp();
			break;

		// inc r16
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
			opcode_inc_r16();
			break;

		// dec r16
		case 0x0B:
		case 0x1B:
		case 0x2B:
		case 0x3B:
			opcode_dec_r16();
			break;

		// add hl, r16
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
			opcode_add_hl_r16();
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
			opcode_inc_r8();
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
			opcode_dec_r8();
			break;

		// ld r8, imm8
		case 0x06:
		case 0x0E:
		case 0x16:
		case 0x1E:
		case 0x26:
		case 0x2E:
		case 0x36:
		case 0x3E:
			opcode_ld_r8_imm8();
			break;

		case 0x07:
			opcode_rlca();
			break;

		case 0x0F:
			opcode_rrca();
			break;

		case 0x17:
			opcode_rla();
			break;

		case 0x1F:
			opcode_rra();
			break;

		case 0x27:
			opcode_daa();
			break;

		case 0x2F:
			opcode_cpl();
			break;

		case 0x37:
			opcode_scf();
			break;

		case 0x3F:
			opcode_ccf();
			break;

		case 0x18:
			opcode_jr_imm8();
			break;

		case 0x20:
		case 0x28:
		case 0x30:
		case 0x38:
			opcode_jr_cond_imm8();
			break;

		case 0x10:
			opcode_stop();
			break;

		// ld r8, r8
		case 0x40:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		case 0x48:
		case 0x49:
		case 0x4A:
		case 0x4B:
		case 0x4C:
		case 0x4D:
		case 0x4E:
		case 0x4F:
		case 0x50:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		case 0x58:
		case 0x59:
		case 0x5A:
		case 0x5B:
		case 0x5C:
		case 0x5D:
		case 0x5E:
		case 0x5F:
		case 0x60:
		case 0x61:
		case 0x62:
		case 0x63:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x68:
		case 0x69:
		case 0x6A:
		case 0x6B:
		case 0x6C:
		case 0x6D:
		case 0x6E:
		case 0x6F:
		case 0x70:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
		case 0x75:
		/*case 0x76: THIS IS HALT OPCODE*/
		case 0x77:
		case 0x78:
		case 0x79:
		case 0x7A:
		case 0x7B:
		case 0x7C:
		case 0x7D:
		case 0x7E:
		case 0x7F:
			opcode_ld_r8_r8();
			break;

		case 0x76:
			opcode_halt();
			break;

		// add a, r8
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
			opcode_add_a_r8();
			break;

		// adc a, r8
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
			opcode_adc_a_r8();
			break;

		// sub a, r8
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
			opcode_sub_a_r8();
			break;

		// sbc a, r8
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
			opcode_sbc_a_r8();
			break;

		// and a, r8
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
			opcode_and_a_r8();
			break;

		// xor a, r8
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
			opcode_xor_a_r8();
			break;

		// or a, r8
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
			opcode_or_a_r8();
			break;

		// cp a, r8
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF:
			opcode_cp_a_r8();
			break;

		case 0xC6:
			opcode_add_a_imm8();
			break;

		case 0xCE:
			opcode_adc_a_imm8();
			break;

		case 0xD6:
			opcode_sub_a_imm8();
			break;

		case 0xDE:
			opcode_sbc_a_imm8();
			break;

		case 0xE6:
			opcode_and_a_imm8();
			break;

		case 0xEE:
			opcode_xor_a_imm8();
			break;

		case 0xF6:
			opcode_or_a_imm8();
			break;

		case 0xFE:
			opcode_cp_a_imm8();
			break;

		// 여기 들어오면 좆된거...
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
