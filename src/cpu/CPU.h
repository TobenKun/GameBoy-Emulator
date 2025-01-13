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

union Interrupt_table
{
	uint8_t value;
	struct
	{
		uint8_t vblank : 1;
		uint8_t lcd : 1;
		uint8_t timer : 1;
		uint8_t serial : 1;
		uint8_t joypad : 1;
		uint8_t unused : 4;
	};
};

class CPU
{
  public:
	CPU();
	~CPU();
	bool LoadROM(const std::string& filename);
	void run();
	void do_opcode();
	void cycle();
	bool interrupt_pending();

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

	Interrupt_table IE;
	Interrupt_table IF;

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

	// rst vector
	std::array<uint8_t, 8> rst = {
		0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
	};

  public:  // instruction test done
	void opcode_dummy();
	void opcode_nope();
	void opcode_ld_r16_imm16();
	void opcode_ld_r16mem_a();
	void opcode_ld_a_r16mem();
	void opcode_ld_imm16_sp();
	void opcode_inc_r16();
	void opcode_dec_r16();
	void opcode_add_hl_r16();
	void opcode_inc_r8();
	void opcode_dec_r8();
	void opcode_ld_r8_imm8();

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

	void opcode_ld_r8_r8();

	void opcode_add_a_r8();
	void opcode_adc_a_r8();
	void opcode_sub_a_r8();
	void opcode_sbc_a_r8();
	void opcode_and_a_r8();
	void opcode_xor_a_r8();
	void opcode_or_a_r8();
	void opcode_cp_a_r8();

	void opcode_add_a_imm8();  // 0xC6
	void opcode_adc_a_imm8();  // 0xCE
	void opcode_sub_a_imm8();  // 0xD6
	void opcode_sbc_a_imm8();  // 0xDE
	void opcode_and_a_imm8();  // 0xE6
	void opcode_xor_a_imm8();  // 0xEE
	void opcode_or_a_imm8();   // 0xF6
	void opcode_cp_a_imm8();   // 0xFE

	void opcode_ret_cond();
	void opcode_ret();
	void opcode_reti();
	void opcode_jp_cond_imm16();
	void opcode_jp_imm16();
	void opcode_jp_hl();
	void opcode_call_cond_imm16();
	void opcode_call_imm16();
	void opcode_rst_tgt3();

	void opcode_pop_r16stk();
	void opcode_push_r16stk();

	void opcode_ldh_c_a();
	void opcode_ldh_imm8_a();
	void opcode_ld_imm16_a();
	void opcode_ldh_a_c();
	void opcode_ldh_a_imm8();
	void opcode_ld_a_imm16();

	void opcode_add_sp_imm8();
	void opcode_ld_hl_sp_imm8();
	void opcode_ld_sp_hl();

	void opcode_di();
	void opcode_ei();

	void opcode_halt();

	void opcode_prefix();

	void prefix_rlc_r8();
	void prefix_rrc_r8();
	void prefix_rl_r8();
	void prefix_rr_r8();
	void prefix_sla_r8();
	void prefix_sra_r8();
	void prefix_swap_r8();
	void prefix_srl_r8();

  public:  // TODO: NEED TEST!!!
	void prefix_bit_b3_r8();
	void prefix_res_b3_r8();
	void prefix_set_b3_r8();

  public:  // for gameboy state
	bool stopped;
	bool halted;
	bool IME;
};
