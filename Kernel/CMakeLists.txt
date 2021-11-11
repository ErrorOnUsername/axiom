set( KERNEL_SOURCES
	Arch/x86_64/CPU.cpp
	Arch/x86_64/Boot/boot.cpp
	Arch/x86_64/GDT/GDT.cpp
	Arch/x86_64/GDT/load_gdt.S
	Arch/x86_64/Interrupts/interrupts.cpp
	Arch/x86_64/Interrupts/interrupts.S
	Arch/x86_64/Interrupts/IDT.cpp
	Memory/MemoryManager.cpp
	Std.cpp
	k_start.cpp
	k_debug.cpp
)

set( KERNEL_HEAP_SOURCES
	Memory/Heap/k_malloc.cpp
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -I -O2" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding -fno-stack-protector -fno-pic -no-pie" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-asynchronous-unwind-tables" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nodefaultlibs -nostdlib" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-80387 -mno-mmx -mno-3dnow -mcmodel=large" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse -mno-sse2 -mno-red-zone" )

link_directories( ${TOOLCHAIN_ROOT}/${SERENITY_ARCH}-pc-elf/lib/ )
link_directories( ${TOOLCHAIN_ROOT}/lib/gcc/${AXIOM_ARCH}-pc-serenity/11.2.0/ )

add_compile_definitions( KERNEL )
add_link_options(LINKER:-z,notext,-z,max-page-size=0x1000)

add_library( k_heap STATIC ${KERNEL_HEAP_SOURCES} )

add_executable( Kernel ${KERNEL_SOURCES} )

target_link_options(Kernel PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib -nodefaultlibs)

target_link_libraries( Kernel PRIVATE gcc k_heap  )
