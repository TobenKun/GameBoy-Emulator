#pragma once

#include <array>
#include <cstdint>
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
};
