#include <kernel/arch/init.hh>

#include <kernel/arch/amd64/interrupts/pic.hh>

namespace Kernel::Arch {

void init()
{
	PIC::init();
}

}
