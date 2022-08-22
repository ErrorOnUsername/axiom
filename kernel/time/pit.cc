#include "pit.hh"

#include <libs/ax/assert.hh>

#include <kernel/arch/cpu.hh>
#include <kernel/arch/interrupts.hh>
#include <kernel/io.hh>
#include <kernel/k_debug.hh>

namespace Kernel::PIT {

static constexpr u8 TIMER_IRQ = 0;

static constexpr u8 TIMER0_PORT      = 0x40;
static constexpr u8 PIT_CONTROL_PORT = 0x43;

// Command Constants
static constexpr u8 TIMER0_SELECT = 0x20;

// PIT Behavior Modes
static constexpr u8 SQUARE_WAVE_MODE = 0x06;

// PIT Access Mode
static constexpr u8 FULL_WORD_ACCESS_MODE = 0x30;

static constexpr size_t BASE_FREQUENCY = 1193182;
static constexpr size_t TICK_RATE      = 1000;

void reset_to_default_frequency();

void init(void (*handler)())
{
	register_irq(TIMER_IRQ, handler);

	IO::out8(PIT_CONTROL_PORT, TIMER0_SELECT | FULL_WORD_ACCESS_MODE | SQUARE_WAVE_MODE);

	size_t value = BASE_FREQUENCY / TICK_RATE;
	IO::out8(TIMER0_PORT, value & 0xff);
	IO::out8(TIMER0_PORT, (value & 0xff) >> 8);

	klogf(LogLevel::Info, "PIT initialized with with frequency: %us Hz", TICK_RATE);
}

}
