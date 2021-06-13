set( KERNEL_SOURCES
	k_init.cpp
	Graphics/VGATextModeDevice.cpp
)

set( KERNEL_SHARED_FLAGS "-fno-builtin -ffreestanding -fno-pie" )
set( KERNEL_SHARED_FLAGS "${KERNEL_SHARED_FLAGS} -nostdinc -nostdinc++ -nostdlib" )
set( KERNEL_SHARED_FLAGS "${KERNEL_SHARED_FLAGS} -mno-red-zone -mcmodel=kernel" )
set( KERNEL_SHARED_FLAGS "${KERNEL_SHARED_FLAGS} -Wall -Wextra" )
set( KERNEL_SHARED_FLAGS "${KERNEL_SHARED_FLAGS} -MMD -I -g3 -ggdb -O2" )

set( CMAKE_CXX_FLAGS "${KERNEL_SHARED_FLAGS}" )

set( CMAKE_ASM_FLAGS "${KERNEL_SHARED_FLAGS} -Wa,--divide" )

add_link_options( LINKER:-T ${CMAKE_CURRENT_BINARY_DIR}/linker.ld -nostdlib -z max-page-size=0x1000 -no-pie LINKER:--build-id=none )

set_source_files_properties( k_init.cpp PROPERTIES
	OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Boot/x86_64/boot.S
)

add_library( boot OBJECT Boot/x86_64/boot.S )

file( GENERATE OUTPUT linker.ld INPUT linker.ld )

add_executable( Kernel ${KERNEL_SOURCES} )
add_dependencies( Kernel boot )
install( TARGETS Kernel RUNTIME DESTINATION boot )
