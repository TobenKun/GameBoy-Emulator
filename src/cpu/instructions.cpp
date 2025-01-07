#include <cstdint>
#include <iostream>
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
void CPU::opcode_dummy() { std::cerr << "Error: Dummy opcode called!\n"; }

void CPU::opcode_nope()	 // 0b00000000
{
	// do nothing
}

void CPU::opcode_LD_r16_imm16()	 // 0x01 0x11 0x21 0x31
{
	int dest = (opcode & 0x30) >> 4;

	uint16_t& target = r16[dest]();
	uint16_t  imm_byte = memory[pc++];
	imm_byte |= (memory[pc++] << 8);  // little endian!!!!!!!

	target = imm_byte;
}

void CPU::opcode_LD_r16mem_A()	// 0x02 0x12 0x22 0x32
{
	int dest = (opcode & 0x30) >> 4;

	uint8_t& target = r16mem[dest]();
	target = A;
}

void CPU::opcode_LD_A_r16mem()	// 0x0A 0x1A 0x2A 0x3A
{
	int src = (opcode & 0x30) >> 4;

	uint8_t& src_byte = r16mem[src]();
	A = src_byte;
}

void CPU::opcode_LD_imm16_sp()	// 0x08
{
	uint16_t imm_byte = memory[pc++];
	imm_byte |= (memory[pc++] << 8);

	// sp 는 16비트니까 반씩 나눠 담는다
	// 리틀 엔디안이니까 하위바이트를 먼저 저장
	memory[imm_byte] = sp & 0xFF;			  // 하위 바이트 저장
	memory[imm_byte + 1] = (sp >> 8) & 0xFF;  // 상위 바이트 저장
}

void CPU::opcode_INC_r16()	// 0x03 0x13 0x23 0x33
{
	int operand = (opcode & 0x30) >> 4;

	uint16_t& target = r16[operand]();
	target++;
}

void CPU::opcode_DEC_r16()	// 0x0B 0x1B 0x2B 0x3B
{
	int operand = (opcode & 0x30) >> 4;

	uint16_t& target = r16[operand]();
	target--;
}

void CPU::opcode_ADD_hl_r16()  // 0x09 0x19 0x29 0x39
{
	int		  operand = (opcode & 0x30) >> 4;
	uint16_t& target = r16[operand]();
	HL.value += target;
	// TODO:: 플래그 레지스터 적당히 조절해줘야함~~!!
	// n->0, 하프캐리, 캐리1
}

void CPU::opcode_INC_r8()  // 0x04 0x0C 0x14 0x1C 0x24 0x2C 0x34 0x3C
{
	int		 operand = (opcode & 0x38) >> 4;
	uint8_t& target = r8[operand]();
	target++;
}

void CPU::opcode_DEC_r8()  // 0x05 0x0D 0x15 0x1D 0x25 0x2D 0x35 0x3D
{
	int		 operand = (opcode & 0x38) >> 4;
	uint8_t& target = r8[operand]();
	target--;
}

void CPU::opcode_LD_r8_imm8()  //
{
	uint8_t operand = (opcode & 0x38) >> 3;
	uint8_t imm_byte = memory[pc++];

	uint8_t& target = r8[operand]();

	target = imm_byte;
}
