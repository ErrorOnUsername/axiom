#include <kernel/arch/processor.hh>

#include <libs/ax/list.hh>
#include <kernel/arch/amd64/boot/tables.hh>
#include <kernel/arch/amd64/msr.hh>

namespace Kernel {

void Processor::initialize(u8 cpu)
{
	cpu_id = cpu;
	idle_thread = nullptr;
	thread = nullptr;

	init_descriptor_tables();

	msr_write(MSR_GS_BASE, (addr_t)this);
}

Thread* Processor::current_thread()
{
	return nullptr /*(Thread*)read_gs_ptr(__builtin_offsetof(Proccessor, thread))*/;
}

Thread* Processor::current_idle_thread()
{
	return nullptr /*(Thread*)read_gs_ptr(__builtin_offsetof(Proccessor, idle_thread))*/;
}

}
