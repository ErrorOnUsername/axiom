#include <kernel/arch/processor.hh>

#include <ax_util/list.hh>

namespace Kernel {

static AX::List<Proccessor*> processors;

void Proccessor::initialize()
{
	processors.ensure_capacity(MAX_PROCESSOR_COUNT);
}

Thread* Proccessor::current_thread() { return nullptr; }

}
