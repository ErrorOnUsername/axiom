#include "paging.hh"

#include <libs/ax/types.hh>

namespace Kernel::Memory {

void switch_address_space(AddressSpace* address_space)
{
	set_address_space((addr_t)address_space);
}

}
