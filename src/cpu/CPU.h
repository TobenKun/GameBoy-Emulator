#pragma once

#include <cstdint>
#include <string>

class CPU
{
  public:
	CPU();
	~CPU();
	bool LoadROM(const std::string& filename);

  private:
	uint8_t memory[0x10000];
};
