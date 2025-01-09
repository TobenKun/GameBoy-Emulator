#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <string>

union Register
{
	uint16_t value;
	struct
	{
		uint8_t lo;
		uint8_t hi;
	};
};

struct Flags
{
	uint8_t unused : 4;
	uint8_t c : 1;
	uint8_t h : 1;
	uint8_t n : 1;
	uint8_t z : 1;
};

class CPU
{
  public:
	CPU();
	~CPU();
	bool LoadROM(const std::string& filename);
	void run();
	void cycle();

  public:  // for debug func
	void PrintRegisters() const;
	void PrintFlags() const;

  public:
	std::array<uint8_t, 0x10000> memory;

	// Register
	union
	{
		Register AF;
		struct
		{
			Flags	F;
			uint8_t A;
		};
	};
	Register BC;
	Register DE;
	Register HL;

	uint16_t sp;
	uint16_t pc;

	uint8_t opcode;

	// get 8-bit register
	std::array<std::function<uint8_t&()>, 8> r8 = {
		[&]() -> uint8_t& { return BC.hi; },
		[&]() -> uint8_t& { return BC.lo; },
		[&]() -> uint8_t& { return DE.hi; },
		[&]() -> uint8_t& { return DE.lo; },
		[&]() -> uint8_t& { return HL.hi; },
		[&]() -> uint8_t& { return HL.lo; },
		[&]() -> uint8_t& { return memory[HL.value]; },
		[&]() -> uint8_t& { return A; },
	};

	// get 16-bit register
	std::array<std::function<uint16_t&()>, 4> r16 = {
		[&]() -> uint16_t& { return BC.value; },
		[&]() -> uint16_t& { return DE.value; },
		[&]() -> uint16_t& { return HL.value; },
		[&]() -> uint16_t& { return sp; },
	};

	// get 16-bit register stack
	std::array<std::function<uint16_t&()>, 4> r16stk = {
		[&]() -> uint16_t& { return BC.value; },
		[&]() -> uint16_t& { return DE.value; },
		[&]() -> uint16_t& { return HL.value; },
		[&]() -> uint16_t& { return AF.value; },
	};

	// get 16-bit register memory -> need to be done in seperate function
	std::array<std::function<uint8_t&()>, 4> r16mem = {
		[&]() -> uint8_t& { return memory[BC.value]; },
		[&]() -> uint8_t& { return memory[DE.value]; },
		[&]() -> uint8_t& { return memory[HL.value++]; },
		[&]() -> uint8_t& { return memory[HL.value--]; },
	};

	// get conditions
	std::array<std::function<bool()>, 4> cond = {
		[&]() -> bool { return !F.z; },
		[&]() -> bool { return F.z; },
		[&]() -> bool { return !F.c; },
		[&]() -> bool { return F.c; },
	};

  public:  // instruction test done
	void opcode_dummy();
	void opcode_nope();
	void opcode_LD_r16_imm16();
	void opcode_LD_r16mem_A();
	void opcode_LD_A_r16mem();
	void opcode_LD_imm16_sp();
	void opcode_INC_r16();
	void opcode_DEC_r16();
	void opcode_ADD_hl_r16();
	void opcode_INC_r8();
	void opcode_DEC_r8();
	void opcode_LD_r8_imm8();

	void opcode_rlca();
	void opcode_rrca();
	void opcode_rla();
	void opcode_rra();
	void opcode_daa();
	void opcode_cpl();
	void opcode_scf();
	void opcode_ccf();

	void opcode_jr_imm8();
	void opcode_jr_cond_imm8();
	void opcode_stop();

  public:  // TODO: NEED TEST!!!
	void opcod
  public:  // for gameboy state
	bool stopped;
};
