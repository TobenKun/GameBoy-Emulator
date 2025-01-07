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

// NOP 명령어 테스트
TEST_F(CPUTest, opcode_NOP)
{
	cpu.memory[0x100] = 0x00;  // NOP 명령어
	cpu.pc = 0x100;			   // 프로그램 카운터 설정

	cpu.cycle();  // 명령어 실행

	EXPECT_EQ(cpu.pc, 0x101);  // pc 1 증가했는지 확인
}
