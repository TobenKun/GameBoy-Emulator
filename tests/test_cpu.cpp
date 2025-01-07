#include <gtest/gtest.h>
#include "CPU.h"

class CPUTest : public ::testing::Test
{
  protected:
	CPU cpu;

	void SetUP() {
		// 테스트 시작 전 초기화 부분
		// cpu인스턴스는 생성시 0으로 초기화됨
		// 다른 값이 필요하면 여기서 변경
	};

	void TesrDown() {
		// 테스트 종료 후 정리 작업
	};
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
	cpu.opcode_nope();	// 명령어 실행

	// 얘는 뭐 확인해야 하는지 모르겠는뎅
}

// LD [r16], imm16
TEST_F(CPUTest, opcode_LD_r16_imm16)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x01;  // LD [BC], imm16
	cpu.memory[0x101] = 0x34;  // imm16 low byte
	cpu.memory[0x102] = 0x12;  // imm16 high byte

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16_imm16();	// Execute instruction

	EXPECT_EQ(cpu.BC.value, 0x1234);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x11;  // LD [DE], imm16
	cpu.memory[0x101] = 0x78;  // imm16 low byte
	cpu.memory[0x102] = 0x56;  // imm16 high byte

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16_imm16();	// Execute instruction

	EXPECT_EQ(cpu.DE.value, 0x5678);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x21;  // LD [HL], imm16
	cpu.memory[0x101] = 0xBC;  // imm16 low byte
	cpu.memory[0x102] = 0x9A;  // imm16 high byte

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16_imm16();	// Execute instruction

	EXPECT_EQ(cpu.HL.value, 0x9ABC);
	EXPECT_EQ(cpu.pc, 0x103);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x31;  // LD [SP], imm16
	cpu.memory[0x101] = 0xF0;  // imm16 low byte
	cpu.memory[0x102] = 0xDE;  // imm16 high byte

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16_imm16();	// Execute instruction

	EXPECT_EQ(cpu.sp, 0xDEF0);
	EXPECT_EQ(cpu.pc, 0x103);
}

TEST_F(CPUTest, opcode_LD_r16mem_A)
{
	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x02;  // LD (BC), A
	cpu.A = 0x12;
	cpu.BC.value = 0x2000;

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16mem_A();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x2000], 0x12);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x12;  // LD (DE), A
	cpu.A = 0x34;
	cpu.DE.value = 0x3000;

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16mem_A();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x3000], 0x34);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x22;  // LD (HL+), A
	cpu.A = 0x56;
	cpu.HL.value = 0x4000;

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16mem_A();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x4000], 0x56);
	EXPECT_EQ(cpu.HL.value, 0x4001);
	EXPECT_EQ(cpu.pc, 0x101);

	cpu.pc = 0x100;
	cpu.memory[0x100] = 0x32;  // LD (HL-), A
	cpu.A = 0x78;
	cpu.HL.value = 0x5000;

	cpu.opcode = cpu.memory[cpu.pc];
	cpu.pc += 1;
	cpu.opcode_LD_r16mem_A();  // Execute instruction

	EXPECT_EQ(cpu.memory[0x5000], 0x78);
	EXPECT_EQ(cpu.HL.value, 0x4FFF);
	EXPECT_EQ(cpu.pc, 0x101);
}
