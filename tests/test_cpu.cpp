#include <gtest/gtest.h>
#include "CPU.h"

class CPUTest : public ::testing::Test
{
  protected:
	CPU cpu;

	void SetUp() override
	{
		// 테스트 시작 전 초기화 부분
		// cpu인스턴스는 생성시 0으로 초기화됨
		// 다른 값이 필요하면 여기서 변경
		cpu.A = 0;
		cpu.F = {0, 0, 0, 0};
	}

	void TearDown() override
	{
		// 테스트 종료 후 정리 작업
	}
};

// 생성자 초기화 테스트
TEST_F(CPUTest, initial_check)
{
	EXPECT_EQ(cpu.AF.value, 0x0);
	EXPECT_EQ(cpu.BC.value, 0x0);
	EXPECT_EQ(cpu.DE.value, 0x0);
	EXPECT_EQ(cpu.HL.value, 0x0);
	EXPECT_EQ(cpu.sp, 0x0);
	EXPECT_EQ(cpu.pc, 0x0);
	EXPECT_EQ(cpu.opcode, 0x0);
}

// NOP 명령어 테스트
TEST_F(CPUTest, opcode_NOP)
{
	cpu.memory[0x100] = 0x00;  // NOP
	cpu.pc = 0x100;
	cpu.cycle();  // 명령어 실행

	EXPECT_EQ(cpu.pc, 0x101);
}

// LD [r16], imm16
TEST_F(CPUTest, opcode_LD_r16_imm16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x01;  // LD [BC], imm16
	cpu.memory[0x101] = 0x34;  // imm16 low byte
	cpu.memory[0x102] = 0x12;  // imm16 high byte

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.value, 0x1234);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x11;  // LD [DE], imm16
	cpu.memory[0x101] = 0x78;  // imm16 low byte
	cpu.memory[0x102] = 0x56;  // imm16 high byte

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.value, 0x5678);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x21;  // LD [HL], imm16
	cpu.memory[0x101] = 0xBC;  // imm16 low byte
	cpu.memory[0x102] = 0x9A;  // imm16 high byte

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x9ABC);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x31;  // LD [SP], imm16
	cpu.memory[0x101] = 0xF0;  // imm16 low byte
	cpu.memory[0x102] = 0xDE;  // imm16 high byte

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.sp, 0xDEF0);
	EXPECT_EQ(cpu.pc, 0x103);
}

TEST_F(CPUTest, opcode_LD_r16mem_A)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x02;  // LD (BC), A
	cpu.A = 0x12;
	cpu.BC.value = 0x2000;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x2000], 0x12);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x12;  // LD (DE), A
	cpu.A = 0x34;
	cpu.DE.value = 0x3000;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x3000], 0x34);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x22;  // LD (HL+), A
	cpu.A = 0x56;
	cpu.HL.value = 0x4000;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x4000], 0x56);
	EXPECT_EQ(cpu.HL.value, 0x4001);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x32;  // LD (HL-), A
	cpu.A = 0x78;
	cpu.HL.value = 0x5000;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x5000], 0x78);
	EXPECT_EQ(cpu.HL.value, 0x4FFF);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_LD_A_r16mem 테스트
TEST_F(CPUTest, opcode_LD_A_r16mem)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0A;  // LD A, (BC)
	cpu.BC.value = 0x2000;
	cpu.memory[0x2000] = 0x12;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x12);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1A;  // LD A, (DE)
	cpu.DE.value = 0x3000;
	cpu.memory[0x3000] = 0x34;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x34);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2A;  // LD A, (HL+)
	cpu.HL.value = 0x4000;
	cpu.memory[0x4000] = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x56);
	EXPECT_EQ(cpu.HL.value, 0x4001);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3A;  // LD A, (HL-)
	cpu.HL.value = 0x5000;
	cpu.memory[0x5000] = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x78);
	EXPECT_EQ(cpu.HL.value, 0x4FFF);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_LD_imm16_sp 테스트
TEST_F(CPUTest, opcode_LD_imm16_sp)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x08;  // LD (imm16), SP
	cpu.memory[0x101] = 0x34;  // imm16 low byte
	cpu.memory[0x102] = 0x12;  // imm16 high byte
	cpu.sp = 0x5678;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x1234], 0x78);
	EXPECT_EQ(cpu.memory[0x1235], 0x56);
	EXPECT_EQ(cpu.pc, 0x103);
}

// opcode_INC_r16 테스트
TEST_F(CPUTest, opcode_INC_r16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x03;  // INC BC
	cpu.BC.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.value, 0x1235);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x13;  // INC DE
	cpu.DE.value = 0x5678;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.value, 0x5679);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x23;  // INC HL
	cpu.HL.value = 0x9ABC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x9ABD);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x33;  // INC SP
	cpu.sp = 0xDEF0;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.sp, 0xDEF1);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_DEC_r16 테스트
TEST_F(CPUTest, opcode_DEC_r16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0B;  // DEC BC
	cpu.BC.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.value, 0x1233);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1B;  // DEC DE
	cpu.DE.value = 0x5678;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.value, 0x5677);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2B;  // DEC HL
	cpu.HL.value = 0x9ABC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x9ABB);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3B;  // DEC SP
	cpu.sp = 0xDEF0;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.sp, 0xDEEF);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_ADD_hl_r16 테스트
TEST_F(CPUTest, opcode_ADD_hl_r16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x09;  // ADD HL, BC
	cpu.HL.value = 0x1234;
	cpu.BC.value = 0x5678;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x68AC);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be unaffected
	EXPECT_EQ(cpu.F.n, 0);	// Subtract flag should be reset
	EXPECT_EQ(cpu.F.h, ((0x1234 & 0xFFF) + (0x5678 & 0xFFF)) & 0x1000
						   ? 1
						   : 0);  // Half carry flag check
	EXPECT_EQ(cpu.F.c, 0);		  // Carry flag should be reset

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x19;  // ADD HL, DE
	cpu.HL.value = 0x9ABC;
	cpu.DE.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0xACF0);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be unaffected
	EXPECT_EQ(cpu.F.n, 0);	// Subtract flag should be reset
	EXPECT_EQ(cpu.F.h, ((0x9ABC & 0xFFF) + (0x1234 & 0xFFF)) & 0x1000
						   ? 1
						   : 0);  // Half carry flag check
	EXPECT_EQ(cpu.F.c, 0);		  // Carry flag should be reset

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x29;  // ADD HL, HL
	cpu.HL.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x2468);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be unaffected
	EXPECT_EQ(cpu.F.n, 0);	// Subtract flag should be reset
	EXPECT_EQ(cpu.F.h, ((0x1234 & 0xFFF) + (0x1234 & 0xFFF)) & 0x1000
						   ? 1
						   : 0);  // Half carry flag check
	EXPECT_EQ(cpu.F.c, 0);		  // Carry flag should be reset

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x39;  // ADD HL, SP
	cpu.HL.value = 0x9ABC;
	cpu.sp = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0xACF0);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be unaffected
	EXPECT_EQ(cpu.F.n, 0);	// Subtract flag should be reset
	EXPECT_EQ(cpu.F.h, ((0x9ABC & 0xFFF) + (0x1234 & 0xFFF)) & 0x1000
						   ? 1
						   : 0);  // Half carry flag check
	EXPECT_EQ(cpu.F.c, 0);		  // Carry flag should be reset
}

// opcode_INC_r8 테스트
TEST_F(CPUTest, opcode_INC_r8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3C;  // INC A
	cpu.A = 0x0F;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x10);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be 0
	EXPECT_EQ(cpu.F.n, 0);	// Subtract flag should be 0
	EXPECT_EQ(cpu.F.h, 1);	// Half-carry flag should be 0

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x04;  // INC B
	cpu.BC.hi = 0xFF;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.hi, 0x00);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 1);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 1);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0C;  // INC C
	cpu.BC.lo = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.lo, 0x57);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x14;  // INC D
	cpu.DE.hi = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.hi, 0x79);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1C;  // INC E
	cpu.DE.lo = 0x9A;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.lo, 0x9B);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x24;  // INC H
	cpu.HL.hi = 0xBC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.hi, 0xBD);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2C;  // INC L
	cpu.HL.lo = 0xDE;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.lo, 0xDF);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

TEST_F(CPUTest, opcode_DEC_r8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3D;  // DEC A
	cpu.A = 0x10;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x0F);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);	// Zero flag should be 0
	EXPECT_EQ(cpu.F.n, 1);	// Subtract flag should be 1
	EXPECT_EQ(cpu.F.h, 1);	// Half-carry flag should be 0

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x05;  // DEC B
	cpu.BC.hi = 0x01;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.hi, 0x00);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 1);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0D;  // DEC C
	cpu.BC.lo = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.lo, 0x55);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x15;  // DEC D
	cpu.DE.hi = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.hi, 0x77);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1D;  // DEC E
	cpu.DE.lo = 0x9A;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.lo, 0x99);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x25;  // DEC H
	cpu.HL.hi = 0xBC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.hi, 0xBB);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2D;  // DEC L
	cpu.HL.lo = 0xDE;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.lo, 0xDD);
	EXPECT_EQ(cpu.pc, 0x101);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);
}

// opcode_LD_r8_imm8 테스트
TEST_F(CPUTest, opcode_LD_r8_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3E;  // LD A, imm8
	cpu.memory[0x101] = 0x12;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x12);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x06;  // LD B, imm8
	cpu.memory[0x101] = 0x34;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.hi, 0x34);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0E;  // LD C, imm8
	cpu.memory[0x101] = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.lo, 0x56);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x16;  // LD D, imm8
	cpu.memory[0x101] = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.hi, 0x78);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1E;  // LD E, imm8
	cpu.memory[0x101] = 0x9A;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.lo, 0x9A);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x26;  // LD H, imm8
	cpu.memory[0x101] = 0xBC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.hi, 0xBC);
	EXPECT_EQ(cpu.pc, 0x102);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2E;  // LD L, imm8
	cpu.memory[0x101] = 0xDE;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.lo, 0xDE);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_rlca)
{
	cpu.A = 0x85;  // 10000101
	cpu.opcode_rlca();
	EXPECT_EQ(cpu.A, 0x0B);	 // 00001011
	EXPECT_EQ(cpu.F.c, 1);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

TEST_F(CPUTest, opcode_rrca)
{
	cpu.A = 0x85;  // 10000101
	cpu.opcode_rrca();
	EXPECT_EQ(cpu.A, 0xC2);	 // 11000010
	EXPECT_EQ(cpu.F.c, 1);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

TEST_F(CPUTest, opcode_rla)
{
	cpu.A = 0x85;  // 10000101
	cpu.F.c = 1;
	cpu.opcode_rla();
	EXPECT_EQ(cpu.A, 0x0B);	 // 00001011
	EXPECT_EQ(cpu.F.c, 1);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

TEST_F(CPUTest, opcode_rra)
{
	cpu.A = 0x85;  // 10000101
	cpu.F.c = 1;
	cpu.opcode_rra();
	EXPECT_EQ(cpu.A, 0xC2);	 // 11000010
	EXPECT_EQ(cpu.F.c, 1);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

// TODO: 다른 BCD 관련 명령어 만들고 나서
// 그거 이용해서 테스트 다시 짜기
TEST_F(CPUTest, opcode_daa)
{
	// 덧셈 이후 DAA 테스트 - 하위 4비트 조정
	cpu.A = 0x0A;  // 하위 4비트가 10 이상
	cpu.F.h = 1;   // 하프 캐리 발생
	cpu.F.c = 0;   // 캐리 없음
	cpu.F.n = 0;   // 덧셈
	cpu.opcode_daa();

	EXPECT_EQ(cpu.A, 0x10);	 // BCD 조정 결과
	EXPECT_EQ(cpu.F.h, 0);	 // 하프 캐리 플래그 리셋
	EXPECT_EQ(cpu.F.c, 0);	 // 캐리 플래그 유지

	// 덧셈 이후 DAA 테스트 - 상위 4비트 조정
	cpu.A = 0x9A;  // 상위 4비트가 10 이상
	cpu.F.h = 1;   // 하프 캐리 발생
	cpu.F.c = 1;   // 캐리 없음
	cpu.F.n = 0;   // 덧셈
	cpu.opcode_daa();

	EXPECT_EQ(cpu.A, 0x00);	 // BCD 조정 결과 (캐리 발생)
	EXPECT_EQ(cpu.F.h, 0);	 // 하프 캐리 플래그 리셋
	EXPECT_EQ(cpu.F.c, 1);	 // 캐리 플래그 설정
	EXPECT_EQ(cpu.F.z, 1);	 // 결과가 0이므로 제로 플래그 설정

	// 뺄셈 이후 DAA 테스트
	cpu.A = 0x1A;  // 초기 값
	cpu.F.h = 1;   // 하프 캐리 발생
	cpu.F.c = 0;   // 캐리 발생
	cpu.F.n = 1;   // 뺄셈
	cpu.opcode_daa();

	EXPECT_EQ(cpu.A, 0x14);	 // BCD 조정 결과
	EXPECT_EQ(cpu.F.h, 0);	 // 하프 캐리 플래그 리셋
	EXPECT_EQ(cpu.F.c, 0);	 // 캐리 플래그 유지
}

TEST_F(CPUTest, opcode_cpl)
{
	cpu.A = 0x85;  // 10000101
	cpu.opcode_cpl();
	EXPECT_EQ(cpu.A, 0x7A);	 // 01111010
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 1);
}

TEST_F(CPUTest, opcode_scf)
{
	cpu.opcode_scf();
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 1);
}

TEST_F(CPUTest, opcode_ccf)
{
	cpu.F.c = 1;
	cpu.opcode_ccf();
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
}

TEST_F(CPUTest, opcode_jr_imm8)
{
	// Test 1: Forward jump
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x18;  // JR opcode
	cpu.memory[0x101] = 0x05;  // Offset +5
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x107);  // PC should jump to 0x100 + 2 + 5

	// Test 2: Backward jump
	cpu.pc = 0x200;
	cpu.memory[0x200] = 0x18;  // JR opcode
	cpu.memory[0x201] = 0xFB;  // Offset -5 (0xFB as signed 8-bit is -5)
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1FD);  // PC should jump to 0x200 + 2 - 5

	// Test 3: No jump (offset = 0)
	cpu.pc = 0x300;
	cpu.memory[0x300] = 0x18;  // JR opcode
	cpu.memory[0x301] = 0x00;  // Offset 0
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x302);  // PC should move to the next instruction
}

TEST_F(CPUTest, opcode_jr_cond_imm8)
{
	// Test 1: Condition met, forward jump
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x20;  // JR NZ, offset
	cpu.memory[0x101] = 0x05;  // Offset +5
	cpu.F.z = false;		   // NZ condition met
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x107);  // PC should jump to 0x100 + 2 + 5

	// Test 2: Condition not met, no jump
	cpu.pc = 0x200;
	cpu.memory[0x200] = 0x20;  // JR NZ, offset
	cpu.memory[0x201] = 0x05;  // Offset +5
	cpu.F.z = true;			   // NZ condition not met
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x202);  // PC should move to the next instruction

	// Test 3: Condition met, backward jump
	cpu.pc = 0x300;
	cpu.memory[0x300] = 0x28;  // JR Z, offset
	cpu.memory[0x301] = 0xFB;  // Offset -5 (0xFB as signed 8-bit is -5)
	cpu.F.z = true;			   // Z condition met
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x2FD);  // PC should jump to 0x300 + 2 - 5

	// Test 4: Condition not met, no jump
	cpu.pc = 0x400;
	cpu.memory[0x400] = 0x28;  // JR Z, offset
	cpu.memory[0x401] = 0xFB;  // Offset -5 (0xFB as signed 8-bit is -5)
	cpu.F.z = false;		   // Z condition not met
	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x402);  // PC should move to the next instruction
}

TEST_F(CPUTest, opcode_stop)
{
	// Test 1: Normal STOP instruction
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x10;  // STOP opcode
	cpu.memory[0x101] = 0x00;  // Second byte

	cpu.cycle();

	EXPECT_TRUE(cpu.stopped);  // CPU should be in stopped state
	EXPECT_EQ(cpu.pc, 0x102);  // PC should advance by 2

	// Test 2: STOP with incorrect second byte
	cpu.pc = 0x200;
	cpu.memory[0x200] = 0x10;  // STOP opcode
	cpu.memory[0x201] = 0xFF;  // Invalid second byte

	cpu.cycle();

	EXPECT_TRUE(cpu.stopped);  // CPU should still stop
	EXPECT_EQ(cpu.pc, 0x202);  // PC should advance by 2
}

TEST_F(CPUTest, opcode_ld_r8_r8)
{
	// LD B, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x41;  // LD B, C
	cpu.BC.lo = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.BC.hi, 0x12);
	EXPECT_EQ(cpu.pc, 0x101);

	// LD D, E
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x53;  // LD D, E
	cpu.DE.lo = 0x34;
	cpu.cycle();
	EXPECT_EQ(cpu.DE.hi, 0x34);
	EXPECT_EQ(cpu.pc, 0x101);

	// LD H, L
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x65;  // LD H, L
	cpu.HL.lo = 0x56;
	cpu.cycle();
	EXPECT_EQ(cpu.HL.hi, 0x56);
	EXPECT_EQ(cpu.pc, 0x101);
}

/* TODO: 인터럽트 구현하고 마저 테스트하기
TEST_F(CPUTest, opcode_halt)
{
	// HALT
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x76;  // HALT opcode
	cpu.cycle();
	EXPECT_TRUE(cpu.halted);
	EXPECT_EQ(cpu.pc, 0x101);
}
*/

TEST_F(CPUTest, opcode_add_a_r8)
{
	// ADD A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x80;  // ADD A, B
	cpu.A = 0x12;
	cpu.BC.hi = 0x34;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x46);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, ((0x12 & 0xF) + (0x34 & 0xF)) & 0x10 ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// ADD A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x81;  // ADD A, C
	cpu.A = 0x12;
	cpu.BC.lo = 0x34;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x46);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, ((0x12 & 0xF) + (0x34 & 0xF)) & 0x10 ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_adc_a_r8)
{
	// ADC A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x88;  // ADC A, B
	cpu.A = 0x12;
	cpu.BC.hi = 0x34;
	cpu.F.c = 1;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x47);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, ((0x12 & 0xF) + (0x34 & 0xF) + 1) & 0x10 ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// ADC A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x89;  // ADC A, C
	cpu.A = 0x12;
	cpu.BC.lo = 0x34;
	cpu.F.c = 1;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x47);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, ((0x12 & 0xF) + (0x34 & 0xF) + 1) & 0x10 ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_sub_a_r8)
{
	// SUB A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x90;  // SUB A, B
	cpu.A = 0x34;
	cpu.BC.hi = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x22);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, ((0x34 & 0xF) < (0x12 & 0xF)) ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// SUB A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x91;  // SUB A, C
	cpu.A = 0x34;
	cpu.BC.lo = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x22);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, ((0x34 & 0xF) < (0x12 & 0xF)) ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_sbc_a_r8)
{
	// SBC A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x98;  // SBC A, B
	cpu.A = 0x34;
	cpu.BC.hi = 0x12;
	cpu.F.c = 1;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x21);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, ((0x34 & 0xF) < (0x12 & 0xF) + 1) ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// SBC A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x99;  // SBC A, C
	cpu.A = 0x34;
	cpu.BC.lo = 0x12;
	cpu.F.c = 1;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x21);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, ((0x34 & 0xF) < (0x12 & 0xF) + 1) ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_and_a_r8)
{
	// AND A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xA0;  // AND A, B
	cpu.A = 0x3C;
	cpu.BC.hi = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x10);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 1);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// AND A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xA1;  // AND A, C
	cpu.A = 0x3C;
	cpu.BC.lo = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x10);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 1);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_xor_a_r8)
{
	// XOR A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xA8;  // XOR A, B
	cpu.A = 0x3C;
	cpu.BC.hi = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x2E);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// XOR A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xA9;  // XOR A, C
	cpu.A = 0x3C;
	cpu.BC.lo = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x2E);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_or_a_r8)
{
	// OR A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xB0;  // OR A, B
	cpu.A = 0x0F;
	cpu.BC.hi = 0xF0;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0xFF);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// OR A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xB1;  // OR A, C
	cpu.A = 0x0F;
	cpu.BC.lo = 0xF0;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0xFF);
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// OR A, A (result should be zero)
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xB7;  // OR A, A
	cpu.A = 0x00;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_EQ(cpu.F.z, 1);
	EXPECT_EQ(cpu.F.n, 0);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_cp_a_r8)
{
	// CP A, B
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xB8;  // CP A, B
	cpu.A = 0x12;
	cpu.BC.hi = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.F.z, 1);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// CP A, C
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xB9;  // CP A, C
	cpu.A = 0x34;
	cpu.BC.lo = 0x12;
	cpu.cycle();
	EXPECT_EQ(cpu.F.z, 0);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, ((0x34 & 0xF) < (0x12 & 0xF)) ? 1 : 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);

	// CP A, A (result should be zero)
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xBF;  // CP A, A
	cpu.A = 0x34;
	cpu.cycle();
	EXPECT_EQ(cpu.F.z, 1);
	EXPECT_EQ(cpu.F.n, 1);
	EXPECT_EQ(cpu.F.h, 0);
	EXPECT_EQ(cpu.F.c, 0);
	EXPECT_EQ(cpu.pc, 0x101);
}

TEST_F(CPUTest, opcode_add_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xC6;  // opcode for ADD A, imm8
	cpu.memory[0x101] = 0x20;  // immediate value
	cpu.A = 0x10;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x30);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_adc_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xCE;  // opcode for ADC A, imm8
	cpu.memory[0x101] = 0x20;  // immediate value
	cpu.A = 0x10;
	cpu.F.c = 0x01;	 // set carry flag
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x31);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_sub_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xD6;  // opcode for SUB A, imm8
	cpu.memory[0x101] = 0x10;  // immediate value
	cpu.A = 0x30;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x20);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_sbc_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xDE;  // opcode for SBC A, imm8
	cpu.memory[0x101] = 0x10;  // immediate value
	cpu.A = 0x30;
	cpu.F.c = 0x01;	 // set carry flag
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x1F);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_and_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xE6;  // opcode for AND A, imm8
	cpu.memory[0x101] = 0x0F;  // immediate value
	cpu.A = 0xF0;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_xor_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xEE;  // opcode for XOR A, imm8
	cpu.memory[0x101] = 0x0F;  // immediate value
	cpu.A = 0xFF;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0xF0);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_or_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xF6;  // opcode for OR A, imm8
	cpu.memory[0x101] = 0x0F;  // immediate value
	cpu.A = 0xF0;
	cpu.cycle();
	EXPECT_EQ(cpu.A, 0xFF);
	EXPECT_EQ(cpu.pc, 0x102);
}

TEST_F(CPUTest, opcode_cp_a_imm8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xFE;  // opcode for CP A, imm8
	cpu.memory[0x101] = 0x20;  // immediate value
	cpu.A = 0x20;
	cpu.cycle();
	EXPECT_EQ(cpu.F.z, 1);	// check zero flag
	EXPECT_EQ(cpu.pc, 0x102);
}

// ret cond 명령어 테스트
TEST_F(CPUTest, opcode_ret_cond)
{
	// 조건이 참일 때
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0xFFFE] = 0x34;	// 하위 바이트
	cpu.memory[0xFFFF] = 0x12;	// 상위 바이트
	cpu.F.z = false;			// 조건 NZ (Not Zero)
	cpu.memory[0x100] = 0xC0;	// opcode for RET NZ

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0x0000);	// 스택 포인터가 증가됨

	// 조건이 거짓일 때
	cpu.pc = 0x200;
	cpu.sp = 0xFFFE;
	cpu.memory[0xFFFE] = 0x34;	// 하위 바이트
	cpu.memory[0xFFFF] = 0x12;	// 상위 바이트
	cpu.F.z = false;			// 조건 Z (Zero)
	cpu.memory[0x200] = 0xC8;	// opcode for RET Z

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x201);	// 프로그램 카운터가 변경되지 않음
	EXPECT_EQ(cpu.sp, 0xFFFE);	// 스택 포인터가 변경되지 않음
}

// ret 명령어 테스트
TEST_F(CPUTest, opcode_ret)
{
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0xFFFE] = 0x34;	// 하위 바이트
	cpu.memory[0xFFFF] = 0x12;	// 상위 바이트
	cpu.memory[0x100] = 0xC9;	// opcode for RET

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0x0000);	// 스택 포인터가 증가됨
}

// reti 명령어 테스트
TEST_F(CPUTest, opcode_reti)
{
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0xFFFE] = 0x34;	// 하위 바이트
	cpu.memory[0xFFFF] = 0x12;	// 상위 바이트
	cpu.memory[0x100] = 0xD9;	// opcode for RETI

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0x0000);			  // 스택 포인터가 증가됨
	EXPECT_TRUE(cpu.interrupt_enalbled);  // 인터럽트가 활성화됨
}

// jp cond, imm16 명령어 테스트
TEST_F(CPUTest, opcode_jp_cond_imm16)
{
	// 조건이 참일 때
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xC2;  // opcode for JP NZ, imm16
	cpu.memory[0x101] = 0x34;  // 하위 바이트
	cpu.memory[0x102] = 0x12;  // 상위 바이트
	cpu.F.z = false;		   // 조건 NZ (Not Zero)

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 지정된 주소로 점프

	// 조건이 거짓일 때
	cpu.pc = 0x200;
	cpu.memory[0x200] = 0xC2;  // opcode for JP NZ, imm16
	cpu.memory[0x201] = 0x34;  // 하위 바이트
	cpu.memory[0x202] = 0x12;  // 상위 바이트
	cpu.F.z = true;			   // 조건 Z (Zero)

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x203);  // 프로그램 카운터가 다음 명령어로 이동
}

// jp imm16 명령어 테스트
TEST_F(CPUTest, opcode_jp_imm16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0xC3;  // opcode for JP imm16
	cpu.memory[0x101] = 0x34;  // 하위 바이트
	cpu.memory[0x102] = 0x12;  // 상위 바이트

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 지정된 주소로 점프
}

// jp hl 명령어 테스트
TEST_F(CPUTest, opcode_jp_hl)
{
	cpu.pc = 0x100;
	cpu.HL.value = 0x1234;
	cpu.memory[0x100] = 0xE9;  // opcode for JP (HL)

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 HL 레지스터의 값으로 점프
}

// call cond, imm16 명령어 테스트
TEST_F(CPUTest, opcode_call_cond_imm16)
{
	// 조건이 참일 때
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0x100] = 0xC4;  // opcode for CALL NZ, imm16
	cpu.memory[0x101] = 0x34;  // 하위 바이트
	cpu.memory[0x102] = 0x12;  // 상위 바이트
	cpu.F.z = false;		   // 조건 NZ (Not Zero)

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 지정된 주소로 점프
	EXPECT_EQ(cpu.memory[0xFFFD], 0x01);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0x03);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨

	// 조건이 거짓일 때
	cpu.pc = 0x200;
	cpu.sp = 0xFFFE;
	cpu.memory[0x200] = 0xC4;  // opcode for CALL NZ, imm16
	cpu.memory[0x201] = 0x34;  // 하위 바이트
	cpu.memory[0x202] = 0x12;  // 상위 바이트
	cpu.F.z = true;			   // 조건 Z (Zero)

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x203);  // 프로그램 카운터가 다음 명령어로 이동
	EXPECT_EQ(cpu.sp, 0xFFFE);	// 스택 포인터가 변경되지 않음
}

// call imm16 명령어 테스트
TEST_F(CPUTest, opcode_call_imm16)
{
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0x100] = 0xCD;  // opcode for CALL imm16
	cpu.memory[0x101] = 0x34;  // 하위 바이트
	cpu.memory[0x102] = 0x12;  // 상위 바이트

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x1234);	// 프로그램 카운터가 지정된 주소로 점프
	EXPECT_EQ(cpu.memory[0xFFFD], 0x01);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0x03);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨
}

// rst tgt3 명령어 테스트
TEST_F(CPUTest, opcode_rst_tgt3)
{
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.memory[0x100] = 0xDF;  // opcode for RST 18H

	cpu.cycle();

	EXPECT_EQ(cpu.pc, 0x18);  // 프로그램 카운터가 0x18로 점프
	EXPECT_EQ(cpu.memory[0xFFFD], 0x01);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0x01);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨
}

// pop r16stk 명령어 테스트
TEST_F(CPUTest, opcode_pop_r16stk)
{
	// POP BC
	cpu.pc = 0x100;
	cpu.sp = 0xFFFC;
	cpu.memory[0xFFFC] = 0x34;	// 하위 바이트
	cpu.memory[0xFFFD] = 0x12;	// 상위 바이트
	cpu.memory[0x100] = 0xC1;	// opcode for POP BC

	cpu.cycle();

	EXPECT_EQ(cpu.BC.value, 0x1234);  // BC 레지스터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0xFFFE);		  // 스택 포인터가 증가됨

	// POP DE
	cpu.pc = 0x100;
	cpu.sp = 0xFFFC;
	cpu.memory[0xFFFC] = 0x78;	// 하위 바이트
	cpu.memory[0xFFFD] = 0x56;	// 상위 바이트
	cpu.memory[0x100] = 0xD1;	// opcode for POP DE

	cpu.cycle();

	EXPECT_EQ(cpu.DE.value, 0x5678);  // DE 레지스터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0xFFFE);		  // 스택 포인터가 증가됨

	// POP HL
	cpu.pc = 0x100;
	cpu.sp = 0xFFFC;
	cpu.memory[0xFFFC] = 0xBC;	// 하위 바이트
	cpu.memory[0xFFFD] = 0x9A;	// 상위 바이트
	cpu.memory[0x100] = 0xE1;	// opcode for POP HL

	cpu.cycle();

	EXPECT_EQ(cpu.HL.value, 0x9ABC);  // HL 레지스터가 스택에서 복원됨
	EXPECT_EQ(cpu.sp, 0xFFFE);		  // 스택 포인터가 증가됨
}

// push r16stk 명령어 테스트
TEST_F(CPUTest, opcode_push_r16stk)
{
	// PUSH BC
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.BC.value = 0x1234;
	cpu.memory[0x100] = 0xC5;  // opcode for PUSH BC

	cpu.cycle();

	EXPECT_EQ(cpu.memory[0xFFFD], 0x12);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0x34);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨

	// PUSH DE
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.DE.value = 0x5678;
	cpu.memory[0x100] = 0xD5;  // opcode for PUSH DE

	cpu.cycle();

	EXPECT_EQ(cpu.memory[0xFFFD], 0x56);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0x78);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨

	// PUSH HL
	cpu.pc = 0x100;
	cpu.sp = 0xFFFE;
	cpu.HL.value = 0x9ABC;
	cpu.memory[0x100] = 0xE5;  // opcode for PUSH HL

	cpu.cycle();

	EXPECT_EQ(cpu.memory[0xFFFD], 0x9A);  // 상위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.memory[0xFFFC], 0xBC);  // 하위 바이트가 스택에 저장됨
	EXPECT_EQ(cpu.sp, 0xFFFC);			  // 스택 포인터가 감소됨
}
