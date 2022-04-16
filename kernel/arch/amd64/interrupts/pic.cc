#include <kernel/arch/pic.hh>

#include <kernel/io.hh>
#include <kernel/k_debug.hh>

namespace Kernel::PIC {

static constexpr u8 PIC1_COMMAND_PORT = 0x20;
static constexpr u8 PIC1_DATA_PORT    = 0x21;
static constexpr u8 PIC1_OFFSET       = 0x20;

static constexpr u8 PIC2_COMMAND_PORT = 0xa0;
static constexpr u8 PIC2_DATA_PORT    = 0xa1;
static constexpr u8 PIC2_OFFSET       = 0x28;

static constexpr u8 PIC_CODE_END_OF_INTERRUPT = 0x20;

static constexpr u8 PIC_CODE_INIT      = 0x10;
static constexpr u8 PIC_CODE_ICW4      = 0x01;
static constexpr u8 PIC_CODE_8086_MODE = 0x01;

void pic_wait(void)
{
	asm volatile("jmp 1f\n\t"
	             "1:\n\t"
	             "    jmp 2f\n\t"
	             "2:");
}

void init()
{
	IO::out8(PIC1_COMMAND_PORT, PIC_CODE_INIT | PIC_CODE_ICW4);
	pic_wait();
	IO::out8(PIC2_COMMAND_PORT, PIC_CODE_INIT | PIC_CODE_ICW4);
	pic_wait();

	IO::out8(PIC1_DATA_PORT, PIC1_OFFSET);
	pic_wait();
	IO::out8(PIC2_DATA_PORT, PIC2_OFFSET);
	pic_wait();

	IO::out8(PIC1_DATA_PORT, 4);
	pic_wait();
	IO::out8(PIC2_DATA_PORT, 2);
	pic_wait();

	IO::out8(PIC1_DATA_PORT, PIC_CODE_8086_MODE);
	pic_wait();
	IO::out8(PIC2_DATA_PORT, PIC_CODE_8086_MODE);
	pic_wait();

	IO::out8(PIC1_DATA_PORT, 0);
	pic_wait();
	IO::out8(PIC2_DATA_PORT, 0);
	pic_wait();

	klog(LogLevel::Info, "PIC initialized");
}

void disable()
{
	IO::out8(PIC1_DATA_PORT, 0xff);
	IO::out8(PIC2_DATA_PORT, 0xff);
}

void enable_irq() { }

void disable_irq() { }

void end_of_interrupt(u8 irq_number)
{
	if(irq_number >= 8)
		IO::out8(PIC2_COMMAND_PORT, PIC_CODE_END_OF_INTERRUPT);

	IO::out8(PIC1_COMMAND_PORT, PIC_CODE_END_OF_INTERRUPT);
}

}
