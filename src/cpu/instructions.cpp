#include <cstdint>
#include "CPU.h"

// Cycle example
//
// void CPU::run()
// {
// 	while (1)
// 	{
// 		opcode = memory[pc];
// 		pc++; // pc 증가 먼저 한번 하고 들어간다
// 		opcode_opcode; // 각자 함수로 점프
// 	}
// }

void CPU::opcode_nope()	 // 0b00000000
{
	// do nothing
}

void CPU::opcode_LD_r16_imm16()	 // 0b00**0001
{
	int dest = (opcode & 0x30) >> 4;

	uint16_t& target = r16[dest]();
	uint16_t  imm_byte = memory[pc++];
	imm_byte |= (memory[pc++] << 8);  // little endian!!!!!!!

	target = imm_byte;
}

void CPU::opcode_LD_r16mem_A()
{
	int dest = (opcode & 0x30) >> 4;

	uint8_t& target = r16mem[dest]();
	target = A;
}

void CPU::opcode_LD_A_r16mem()
{
	int src = (opcode & 0x30) >> 4;

	uint8_t& src_byte = r16mem[src]();
	A = src_byte;
}

void CPU::opcode_LD_imm16_sp()
{
	uint16_t imm_byte = memory[pc++] << 8;
	imm_byte |= memory[pc++];

	memory[imm_byte] = sp & 0x0F;
	memory[imm_byte + 1] = sp & 0xF0;
}

void CPU::opcode_INC_r16()
{
	uint16_t operand = (opcode & 0x38) >> 3;

	uint16_t& target = r16[operand]();
	target++;
}

void CPU::opcode_DEC_r16()
{
	uint16_t operand = (opcode & 0x38) >> 3;

	uint16_t& target = r16[operand]();
	target--;
}

void CPU::opcode_LD_r8_imm8()
{
	uint8_t operand = (opcode & 0x38) >> 3;
	uint8_t imm_byte = memory[pc++];

	uint8_t& target = r8[operand]();

	target = imm_byte;
}
