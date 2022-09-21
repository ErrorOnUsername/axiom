#include <kernel/arch/processor.hh>

#include <libs/ax/list.hh>

#include <kernel/arch/amd64/boot/tables.hh>
#include <kernel/arch/amd64/asm.hh>
#include <kernel/arch/amd64/msr.hh>

namespace Kernel {

void Processor::initialize(u8 cpu)
{
	cpu_id = cpu;
	idle_thread = nullptr;
	thread = nullptr;

	init_descriptor_tables();

	MSR::write(MSR_GS_BASE, (addr_t)this);
}

Thread* Processor::current_thread()
{
	return (Thread*)read_gs_ptr(__builtin_offsetof(Processor, thread));
}

Thread* Processor::current_idle_thread()
{
	return (Thread*)read_gs_ptr(__builtin_offsetof(Processor, idle_thread));
}

}
