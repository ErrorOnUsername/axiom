set( KERNEL_SOURCES
	arch/amd64/cpu.cc
	arch/amd64/paging.cc
	arch/amd64/paging_utils.S
	arch/amd64/boot/boot_stivale.cc
	arch/amd64/gdt/gdt.cc
	arch/amd64/gdt/load_gdt.S
	arch/amd64/interrupts/interrupts.cc
	arch/amd64/interrupts/interrupts.S
	arch/amd64/interrupts/IDT.cc
	memory/bootloader_memory_map.cc
	memory/memory_manager.cc
	memory/physical.cc
	memory/region.cc
	system/early_console/early_console.cc
	std.cc
	k_start.cc
	k_debug.cc
)

set( KERNEL_HEAP_SOURCES
	memory/heap/k_malloc.cc
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -I -O2" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding -fno-stack-protector -fno-pic -no-pie" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-asynchronous-unwind-tables -fno-rtti -fbuiltin" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-80387 -mno-mmx -mno-3dnow -mcmodel=kernel" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse -mno-sse2 -mno-red-zone" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nodefaultlibs -nostdlib" )

link_directories( ${TOOLCHAIN_ROOT}/${SERENITY_ARCH}-pc-elf/lib/ )
link_directories( ${TOOLCHAIN_ROOT}/lib/gcc/${AXIOM_ARCH}-pc-serenity/11.2.0/ )

add_compile_definitions( KERNEL )
add_link_options(LINKER:-z,notext,-z,max-page-size=0x1000)

add_library( k_heap STATIC ${KERNEL_HEAP_SOURCES} )

add_executable( kernel ${KERNEL_SOURCES} )

target_link_options(kernel PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib -nodefaultlibs)

target_link_libraries( kernel PRIVATE gcc k_heap  )
