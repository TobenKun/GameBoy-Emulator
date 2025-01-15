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
void CPU::opcode_dummy()
{
	std::cerr << "\033[1;33m" << "Error: Dummy opcode called!\n"
			  << "\033[1;0m";
}

void CPU::opcode_nope()	 // 0b00000000
{
	// do nothing
}

void CPU::opcode_ld_r16_imm16()	 // 0x01 0x11 0x21 0x31
{
	int dest = (opcode & 0x30) >> 4;

	uint16_t& target = r16[dest]();
	uint16_t  imm_byte = memory[pc++];
	imm_byte |= (memory[pc++] << 8);  // little endian!!!!!!!

	target = imm_byte;
}

void CPU::opcode_ld_r16mem_a()	// 0x02 0x12 0x22 0x32
{
	int dest = (opcode & 0x30) >> 4;

	uint8_t& target = r16mem[dest]();
	target = A;
}

void CPU::opcode_ld_a_r16mem()	// 0x0A 0x1A 0x2A 0x3A
{
	int src = (opcode & 0x30) >> 4;

	uint8_t& src_byte = r16mem[src]();
	A = src_byte;
}

void CPU::opcode_ld_imm16_sp()	// 0x08
{
	uint16_t imm_byte = memory[pc++];
	imm_byte |= (memory[pc++] << 8);

	// sp 는 16비트니까 반씩 나눠 담는다
	// 리틀 엔디안이니까 하위바이트를 먼저 저장
	memory[imm_byte] = sp & 0xFF;			  // 하위 바이트 저장
	memory[imm_byte + 1] = (sp >> 8) & 0xFF;  // 상위 바이트 저장
}

void CPU::opcode_inc_r16()	// 0x03 0x13 0x23 0x33
{
	int operand = (opcode & 0x30) >> 4;

	uint16_t& target = r16[operand]();
	target++;
}

void CPU::opcode_dec_r16()	// 0x0B 0x1B 0x2B 0x3B
{
	int operand = (opcode & 0x30) >> 4;

	uint16_t& target = r16[operand]();
	target--;
}

void CPU::opcode_add_hl_r16()
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

void CPU::opcode_inc_r8()
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

void CPU::opcode_dec_r8()  // 0x05 0x0D 0x15 0x1D 0x25 0x2D 0x35 0x3D
{
	int		 operand = opcode >> 3;
	uint8_t& target = r8[operand]();
	target--;

	F.z = target == 0;
	F.n = 1;
	F.h = (target + 1 & 0x0F) == 0x00;
}

void CPU::opcode_ld_r8_imm8()  // 0x06 0x0E 0x16 0x1E 0x26 0x2E 0x36 0x3E
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

void CPU::opcode_jr_imm8()
// 0x18
// Flags: none
{
	int8_t offset = static_cast<int8_t>(memory[pc++]);
	pc += offset;
}

void CPU::opcode_jr_cond_imm8()
// 0x20 0x28 0x30 0x38
// Flags: none
{
	uint8_t condition = (opcode >> 3) & 0x3;
	int8_t	offset = memory[pc++];

	if (cond[condition]()) pc += offset;
}

void CPU::opcode_stop()
// 0x10;
// Flags: none
{
	// second byte must be 0x00
	uint8_t second_byte = memory[pc++];
	if (second_byte != 0x00)
	{
		std::cerr
			<< "Warning: STOP instruction second byte is not 0x00. Found: "
			<< std::hex << static_cast<int>(second_byte) << std::endl;
	}

	// GBC double speed mode handler should be here if needed

	stopped = true;
}

void CPU::opcode_ld_r8_r8()
// 0x40 - 0x7F except 0x76
// Flags: none
{
	int src_operand = opcode & 0x7;
	int dest_operand = (opcode >> 3) & 0x7;

	uint8_t& src = r8[src_operand]();
	uint8_t& dest = r8[dest_operand]();

	dest = src;
}

void CPU::opcode_halt()
// 0x76
// Flags: none
{
	halted = true;
}

void CPU::opcode_add_a_r8()
// 0x80 - 0x87
// Flags: Z N H C
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t	 value = r8[operand]();
	uint16_t result = A + value;

	// Set flags
	F.z = (result & 0xFF) == 0;	 // Zero flag
	F.n = 0;					 // Add operation, so N flag is cleared
	F.h = ((A & 0xF) + (value & 0xF)) > 0xF;  // Half carry flag
	F.c = result > 0xFF;					  // Carry flag

	A = result & 0xFF;	// Store the result back in the accumulator
}

void CPU::opcode_adc_a_r8()
// 0x88 - 0x8F
// Flags: Z N H C
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t	 value = r8[operand]();
	uint16_t result = A + value + F.c;

	// Set flags
	F.z = (result & 0xFF) == 0;	 // Zero flag
	F.n = 0;					 // Add operation, so N flag is cleared
	F.h = ((A & 0xF) + (value & 0xF)) > 0xF;  // Half carry flag
	F.c = result > 0xFF;					  // Carry flag

	A = result & 0xFF;	// Store the result back in the accumulator
}

void CPU::opcode_sub_a_r8()
// 0x90 - 0x97
// Flags: Z N H C
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t	 value = r8[operand]();
	uint16_t result = A - value;

	F.z = (result & 0xFF) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF);
	F.c = value > A;

	A = result & 0xFF;
}

void CPU::opcode_sbc_a_r8()
// 0x98 - 0x9F
// Flags: Z N H C
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t	 value = r8[operand]();
	uint16_t result = A - value - F.c;

	F.z = (result & 0xFF) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF) + F.c;
	F.c = value + F.c > A;

	A = result & 0xFF;
}

void CPU::opcode_and_a_r8()
// 0xA0 - 0xA7
// Flags: Z N H C
{
	uint8_t operand = opcode & 0x7;
	uint8_t value = r8[operand]();

	F.z = (A & value) == 0;
	F.n = 0;
	F.h = 1;
	F.c = 0;

	A &= value;
}

void CPU::opcode_xor_a_r8()
// 0xA8 - 0xAF
// Flags: Z N H C
{
	uint8_t operand = opcode & 0x7;
	uint8_t value = r8[operand]();

	F.z = (A ^ value) == 0;
	F.n = 0;
	F.h = 0;
	F.c = 0;

	A ^= value;
}

void CPU::opcode_or_a_r8()
// 0xB0 - 0xB8
// Flags: Z N H C
{
	uint8_t operand = opcode & 0x7;
	uint8_t value = r8[operand]();

	F.z = (A | value) == 0;
	F.n = 0;
	F.h = 0;
	F.c = 0;

	A |= value;
}

void CPU::opcode_cp_a_r8()
// 0xB8 - 0xBF
// Flags: Z N H C
{
	uint8_t operand = opcode & 0x7;
	uint8_t value = r8[operand]();

	F.z = (A - value) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF);
	F.c = value > A;
}

void CPU::opcode_add_a_imm8()
// 0xC6
// Flags: Z N H C
{
	uint8_t	 value = memory[pc++];
	uint16_t result = A + value;

	// Set flags
	F.z = (result & 0xFF) == 0;	 // Zero flag
	F.n = 0;					 // Add operation, so N flag is cleared
	F.h = ((A & 0xF) + (value & 0xF)) > 0xF;  // Half carry flag
	F.c = result > 0xFF;					  // Carry flag

	A = result & 0xFF;	// Store the result back in the accumulator
}

void CPU::opcode_adc_a_imm8()
// 0xCE
// Flags: Z N H C
{
	uint8_t	 value = memory[pc++];
	uint16_t result = A + value + F.c;

	// Set flags
	F.z = (result & 0xFF) == 0;	 // Zero flag
	F.n = 0;					 // Add operation, so N flag is cleared
	F.h = ((A & 0xF) + (value & 0xF)) > 0xF;  // Half carry flag
	F.c = result > 0xFF;					  // Carry flag

	A = result & 0xFF;	// Store the result back in the accumulator
}

void CPU::opcode_sub_a_imm8()
// 0xD6
// Flags: Z N H C
{
	uint8_t	 value = memory[pc++];
	uint16_t result = A - value;

	F.z = (result & 0xFF) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF);
	F.c = value > A;

	A = result & 0xFF;
}

void CPU::opcode_sbc_a_imm8()
// 0xDE
// Flags: Z N H C
{
	uint8_t	 value = memory[pc++];
	uint16_t result = A - value - F.c;

	F.z = (result & 0xFF) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF) + F.c;
	F.c = value + F.c > A;

	A = result & 0xFF;
}

void CPU::opcode_and_a_imm8()
// 0xDE
// Flags: Z N H C
{
	uint8_t value = memory[pc++];

	F.z = (A & value) == 0;
	F.n = 0;
	F.h = 1;
	F.c = 0;

	A &= value;
}

void CPU::opcode_xor_a_imm8()
// 0xEE
// Flags: Z N H C
{
	uint8_t value = memory[pc++];

	F.z = (A ^ value) == 0;
	F.n = 0;
	F.h = 0;
	F.c = 0;

	A ^= value;
}

void CPU::opcode_or_a_imm8()
// 0xF6
// Flags: Z N H C
{
	uint8_t value = memory[pc++];

	F.z = (A | value) == 0;
	F.n = 0;
	F.h = 0;
	F.c = 0;

	A |= value;
}

void CPU::opcode_cp_a_imm8()
// 0xFE
// Flags: Z N H C
{
	uint8_t value = memory[pc++];

	F.z = (A - value) == 0;
	F.n = 1;
	F.h = (A & 0xF) < (value & 0xF);
	F.c = value > A;
}

void CPU::opcode_ret_cond()
// 0xC0 0xC8 0xD0 0xD8
{
	uint8_t condition = (opcode >> 3) & 0x3;

	if (cond[condition]())
	{
		pc = memory[sp++];
		pc |= (memory[sp++] << 8);
	}
}

void CPU::opcode_ret()
// 0xC9
{
	pc = memory[sp++];
	pc |= (memory[sp++] << 8);
}

void CPU::opcode_reti()
// 0xD9
{
	pc = memory[sp++];
	pc |= (memory[sp++] << 8);

	IME = true;
}

void CPU::opcode_jp_cond_imm16()
// 0xC2 0xCA 0xD2 0xDA
// Flags: none
{
	uint8_t	 condition = (opcode >> 3) & 0x3;
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	if (cond[condition]())
	{
		pc = address;
	}
}

void CPU::opcode_jp_imm16()
// 0xC3
// Flags: none
{
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	pc = address;
}

void CPU::opcode_jp_hl()
// 0xE9
{
	pc = HL.value;
}

void CPU::opcode_call_cond_imm16()
// 0xC4 0xCC 0xD4 0xDC
{
	uint8_t	 condition = (opcode >> 3) & 0x3;
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	if (cond[condition]())
	{
		memory[--sp] = (pc & 0xFF00) >> 8;
		memory[--sp] = (pc & 0x00FF);
		pc = address;
	}
}

void CPU::opcode_call_imm16()
// 0xCD
{
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	memory[--sp] = (pc & 0xFF00) >> 8;
	memory[--sp] = (pc & 0x00FF);
	pc = address;
}

void CPU::opcode_rst_tgt3()
// 0xC7 0xCF 0xD7 0xDF 0xE7 0xEF 0xF7 0xFF
// Flags: none
{
	uint8_t target = (opcode >> 3) & 0x7;
	uint8_t target_addr = rst[target];

	memory[--sp] = (pc & 0xFF00) >> 8;
	memory[--sp] = (pc & 0x00FF);
	pc = target_addr;
}

void CPU::opcode_pop_r16stk()
// 0xC1 0xD1 0xE1 0xF1
// Flags: none
{
	uint8_t	  operand = (opcode >> 4) & 0x3;
	uint16_t& target = r16stk[operand]();

	target = memory[sp++];
	target |= (memory[sp++] << 8);
}

void CPU::opcode_push_r16stk()
// 0xC5 0xD5 0xE5 0xF5
// Flags: none
{
	uint8_t	 operand = (opcode >> 4) & 0x3;
	uint16_t target = r16stk[operand]();

	memory[--sp] = (target & 0xFF00) >> 8;
	memory[--sp] = (target & 0x00FF);
}

void CPU::opcode_prefix()
{
	opcode = memory[pc++];
	if (opcode < 0x08)
		prefix_rlc_r8();
	else if (opcode < 0x10)
		prefix_rrc_r8();
	else if (opcode < 0x18)
		prefix_rl_r8();
	else if (opcode < 0x20)
		prefix_rr_r8();
	else if (opcode < 0x28)
		prefix_sla_r8();
	else if (opcode < 0x30)
		prefix_sra_r8();
	else if (opcode < 0x38)
		prefix_swap_r8();
	else if (opcode < 0x40)
		prefix_srl_r8();
	else if (opcode < 0x80)
		prefix_bit_b3_r8();
	else if (opcode < 0xC0)
		prefix_res_b3_r8();
	else
		prefix_set_b3_r8();
}

void CPU::opcode_ldh_c_a()
// 0xE2
// Flags: none
{
	uint16_t address = 0xFF00 + BC.lo;

	memory[address] = A;
}

void CPU::opcode_ldh_imm8_a()
// 0xE0
// Flags: none
{
	uint16_t address = 0xFF00 + memory[pc++];

	memory[address] = A;
}

void CPU::opcode_ld_imm16_a()
// 0xEA
{
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	memory[address] = A;
}

void CPU::opcode_ldh_a_c()
// 0xF2
// Flags: none
{
	uint16_t address = 0xFF00 + BC.lo;

	A = memory[address];
}

void CPU::opcode_ldh_a_imm8()
// 0xF0
{
	uint16_t address = 0xFF00 + memory[pc++];

	A = memory[address];
}

void CPU::opcode_ld_a_imm16()
// 0xFA
{
	uint16_t address = memory[pc++];
	address |= (memory[pc++] << 8);

	A = memory[address];
}

void CPU::opcode_add_sp_imm8()
// 0xE8
// Flags: z n h c
{
	int8_t	 value = static_cast<int8_t>(memory[pc++]);
	uint16_t result = sp + value;

	// set flags
	F.z = 0;
	F.n = 0;
	F.h = ((sp & 0x0F) + (value & 0x0F)) > 0x0F;
	F.c = ((sp & 0xFF) + (value & 0xFF)) > 0xFF;

	sp = result;
}

void CPU::opcode_ld_hl_sp_imm8()
// 0xF8
// Flags: z n h c
{
	int8_t	value = static_cast<int8_t>(memory[pc++]);
	int16_t result = sp + value;

	// set flags
	F.z = 0;
	F.n = 0;
	F.h = ((sp & 0x0F) + (value & 0x0F)) > 0x0F;
	F.c = ((sp & 0xFF) + (value & 0xFF)) > 0xFF;

	HL.value = result;
}

void CPU::opcode_ld_sp_hl()
// 0xF9
// Flags: none
{
	sp = HL.value;
}

void CPU::opcode_di()
// 0xF3
// Flags: none
{
	IME = false;
}

void CPU::opcode_ei()
// 0xFB
// Flags: none
{
	IME = true;
}

void CPU::prefix_rlc_r8()
// 0x00 - 0x07
// Flags: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();

	F.c = (target & 0x80) >> 7;
	target <<= 1;
	target |= F.c;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_rrc_r8()
// 0x08 - 0x0F
// Flags: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();

	F.c = target & 0x01;
	target >>= 1;
	target |= F.c << 7;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_rl_r8()
// 0x10 - 0x17
// Flags: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();
	bool	 extra_bit = F.c;

	F.c = (target & 0x80) >> 7;
	target <<= 1;
	target |= extra_bit;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_rr_r8()
// 0x18 - 0x1F
// Flags:: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();
	bool	 extra_bit = F.c;

	F.c = target & 0x01;
	target >>= 1;
	target |= extra_bit << 7;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_sla_r8()
// 0x20 - 0x27
// Flags:: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();

	F.c = (target & 0x80) >> 7;
	target <<= 1;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_sra_r8()
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();

	F.c = target & 0x01;
	target = (target >> 1) |
			 (target & 0x80);  // 오른쪽으로 시프트하고 왼쪽 비트를 유지

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_swap_r8()
// 0x30 - 0x37
// Flags: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();
	uint8_t	 tmp = target & 0xF0;

	target <<= 4;
	target |= tmp >> 4;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
	F.c = 0;
}

void CPU::prefix_srl_r8()
// 0x38 - 0x3F
// Flags: z n h c
{
	uint8_t	 operand = opcode & 0x7;
	uint8_t& target = r8[operand]();

	F.c = target & 0x01;
	target >>= 1;

	// flag adjustment
	F.z = target == 0;
	F.n = 0;
	F.h = 0;
}

void CPU::prefix_bit_b3_r8()
// 0x40 - 0x7F
// Flags: z n h
{
	uint8_t	 target = opcode & 0x07;
	uint8_t& target_register = r8[target]();
	uint8_t	 bit = (opcode >> 3) & 0x07;
	uint8_t	 value = target_register & (1 << bit);

	// flag adjustment
	F.z = value == 0;
	F.n = 0;
	F.h = 1;
}

void CPU::prefix_res_b3_r8()
// 0x80 - 0xBF
// Flags: none
{
	uint8_t	 target = opcode & 0x07;
	uint8_t& target_register = r8[target]();
	uint8_t	 bit = (opcode >> 3) & 0x07;

	target_register &= ~(1 << bit);
}

void CPU::prefix_set_b3_r8()
// 0xC0 - 0xFF
// Flags: none
{
	uint8_t	 target = opcode & 0x07;
	uint8_t& target_register = r8[target]();
	uint8_t	 bit = (opcode >> 3) & 0x07;

	target_register |= (1 << bit);
}
