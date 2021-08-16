set( KERNEL_SOURCES
	k_start.cpp
	k_debug.cpp
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I -O2" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding -fno-stack-protector -fno-pic -fpie" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone" )

add_link_options( LINKER: -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
	-fno-pic -fpie
	-Wl,-static,-pie -static-pie
	-nostdlib -z max-page-size=0x1000
)

add_executable( Kernel ${KERNEL_SOURCES} )
