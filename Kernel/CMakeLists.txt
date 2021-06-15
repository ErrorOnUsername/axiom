set( KERNEL_SOURCES
	k_start.cpp
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

add_link_options( LINKER: -T linker.ld )

add_executable( Kernel ${KERNEL_SOURCES} )
