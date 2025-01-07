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

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x19;  // ADD HL, DE
	cpu.HL.value = 0x9ABC;
	cpu.DE.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0xACE0);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x29;  // ADD HL, HL
	cpu.HL.value = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x2468);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x39;  // ADD HL, SP
	cpu.HL.value = 0x9ABC;
	cpu.sp = 0x1234;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.value, 0xACE0);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_INC_r8 테스트
TEST_F(CPUTest, opcode_INC_r8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3C;  // INC A
	cpu.A = 0x12;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x13);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x04;  // INC B
	cpu.BC.hi = 0x34;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.hi, 0x35);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0C;  // INC C
	cpu.BC.lo = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.lo, 0x57);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x14;  // INC D
	cpu.DE.hi = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.hi, 0x79);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1C;  // INC E
	cpu.DE.lo = 0x9A;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.lo, 0x9B);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x24;  // INC H
	cpu.HL.hi = 0xBC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.hi, 0xBD);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2C;  // INC L
	cpu.HL.lo = 0xDE;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.lo, 0xDF);
	EXPECT_EQ(cpu.pc, 0x101);
}

// opcode_DEC_r8 테스트
TEST_F(CPUTest, opcode_DEC_r8)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x3D;  // DEC A
	cpu.A = 0x12;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.A, 0x11);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x05;  // DEC B
	cpu.BC.hi = 0x34;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.hi, 0x33);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x0D;  // DEC C
	cpu.BC.lo = 0x56;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.BC.lo, 0x55);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x15;  // DEC D
	cpu.DE.hi = 0x78;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.hi, 0x77);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x1D;  // DEC E
	cpu.DE.lo = 0x9A;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.DE.lo, 0x99);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x25;  // DEC H
	cpu.HL.hi = 0xBC;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.hi, 0xBB);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x2D;  // DEC L
	cpu.HL.lo = 0xDE;

	cpu.cycle();  // Execute instruction

	EXPECT_EQ(cpu.HL.lo, 0xDD);
	EXPECT_EQ(cpu.pc, 0x101);
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
