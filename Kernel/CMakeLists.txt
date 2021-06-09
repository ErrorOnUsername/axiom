set( KERNEL_SOURCES
	k_init.cpp
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie -fPIE" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -ffreestanding -fbuiltin" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-mmx -mno-sse -mno-sse2" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=large -mno-red-zone" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -nostdinc -nostdinc++" )

add_link_options( LINKER:-T ${CMAKE_CURRENT_BINARY_DIR}/linker.ld -nostdlib )

set_source_files_properties( k_init.cpp PROPERTIES
	OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Boot/x86_64/boot.S
)

add_library( boot OBJECT Boot/x86_64/boot.S )

file( GENERATE OUTPUT linker.ld INPUT linker.ld )

add_executable( Kernel ${KERNEL_SOURCES} )
add_dependencies( Kernel boot )
install( TARGETS Kernel RUNTIME DESTINATION boot )
