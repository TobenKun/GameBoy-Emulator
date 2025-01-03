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

  public:  // for debug func
	void PrintRegisters() const;
	void PrintFlags() const;

  public:
	std::array<uint8_t, 0x10000> memory;

  private:	// Instruction
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
		[&]() -> uint8_t& { return BC.lo; },
		[&]() -> uint8_t& { return BC.hi; },
		[&]() -> uint8_t& { return DE.lo; },
		[&]() -> uint8_t& { return DE.hi; },
		[&]() -> uint8_t& { return HL.lo; },
		[&]() -> uint8_t& { return HL.hi; },
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
};
