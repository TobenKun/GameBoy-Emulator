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

void CPU::opcode_ADD_hl_r16()
// 0x09 0x19 0x29 0x39
// Flags: N, H, C
{
	int		  operand = (opcode & 0x30) >> 4;
	uint16_t& target = r16[operand]();
	int		  result = HL.value + target;

	// flag adjustment
	F.n = 0;
	F.c = result > 0xFFFF;
	F.h = (HL.value & 0xFFF + target & 0xFFF) > 0xFFF;

	HL.value = result;
}

void CPU::opcode_INC_r8()
// 0x04 0x0C 0x14 0x1C 0x24 0x2C 0x34 0x3C
// Flags: Z, N, H
{
	int		 operand = opcode >> 3;
	uint8_t& target = r8[operand]();
	target++;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = (target & 0x0F) == 0x00;
}

void CPU::opcode_DEC_r8()  // 0x05 0x0D 0x15 0x1D 0x25 0x2D 0x35 0x3D
{
	int		 operand = opcode >> 3;
	uint8_t& target = r8[operand]();
	target--;

	F.z = target == 0;
	F.n = 1;
	F.h = (target + 1 & 0x0F) == 0x00;
}

void CPU::opcode_LD_r8_imm8()  // 0x06 0x0E 0x16 0x1E 0x26 0x2E 0x36 0x3E
{
	uint8_t operand = (opcode & 0x38) >> 3;
	uint8_t imm_byte = memory[pc++];

	uint8_t& target = r8[operand]();

	target = imm_byte;
}

void CPU::opcode_rlca()
// 0x07
// Flags: z, n, h, c
{
	F.c = (A & 0x80) >> 7;
	A <<= 1;
	A |= F.c;

	// flag adjustment
	F.z = 0;
	F.n = 0;
	F.h = 0;
}

void CPU::opcode_rrca()
// 0x0F
// Flags: z, n, h, c
{
	F.c = A & 0x01;
	A >>= 1;
	A |= F.c << 7;

	// flag adjustment
	F.z = 0;
	F.n = 0;
	F.h = 0;
}

void CPU::opcode_rla()
// 0x17
// Flags: z, n, h, c
{
	bool extra_bit = F.c;
	F.c = (A & 0x80) >> 7;
	A <<= 1;
	A |= extra_bit;

	// flag adjustment
	F.z = 0;
	F.n = 0;
	F.h = 0;
}

void CPU::opcode_rra()
// 0x1F
// Flags: z, n, h, C
{
	bool extra_bit = F.c;
	F.c = A & 0x01;
	A >>= 1;
	A |= extra_bit << 7;

	// flag adjustment
	F.z = 0;
	F.n = 0;
	F.h = 0;
}

void CPU::opcode_daa()
// 0x27
// Flags z, h, c
{
	int adjustment = 0;

	switch (F.n)
	{
		case 1:
			adjustment = 0x6 * F.h + 0x60 * F.c;
			F.c = adjustment > A;
			A -= adjustment;
			break;
		case 0:
			adjustment += 0x6 * (F.h | ((A & 0xF) > 0x9));
			adjustment += 0x60 * (F.c | (A > 0x9F));
			int result = A + adjustment;
			F.c = result > 0xFF;
			A = result;
			break;
	}
	F.z = A == 0;
	F.h = 0;
}

void CPU::opcode_cpl()
// 0x2F
// Flags: N, H
{
	A = ~A;
	F.n = 1;
	F.h = 1;
}

void CPU::opcode_scf()
// 0x37
// Flags: n h c
{
	F.n = 0;
	F.h = 0;
	F.c = 1;
}

void CPU::opcode_ccf()
// 0x3F
// Flags: n h c
{
	F.n = 0;
	F.h = 0;
	F.c = ~F.c;
}

// TODO: ---------여기서부터 테스트 필요-------------
// 08/01/2025
void CPU::opcode_jr_imm8()
// 0x18
// Flags: none
{
	int8_t offset = memory[pc++];
	pc += offset;
}  // TODO:: 스위치문 연결 해야됨

void CPU::opcode_jr_cond_imm8()
{
	// something
}

void CPU::opcode_stop()
{
	// something
}
