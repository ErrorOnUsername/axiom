cmake_minimum_required( VERSION 3.13 )
project( axiom ASM CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( AXIOM_ARCH x86_64 )

add_compile_definitions( AXIOM_ARCH_AMD64 )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_custom_target( img
	COMMAND make -C ../vendor/limine
	COMMAND mkdir -p iso_root
	COMMAND cp -v kernel/kernel ../limine.cfg ../vendor/limine/limine.sys ../vendor/limine/limine-cd.bin
	              ../vendor/limine/limine-eltorito-efi.bin iso_root
	COMMAND xorriso -as mkisofs -b limine-cd.bin
	                -no-emul-boot -boot-load-size 4 -boot-info-table
	                --efi-boot limine-eltorito-efi.bin
	                -efi-boot-part --efi-boot-image --protective-msdos-label
					iso_root -o _qemu_img
	COMMAND ./../vendor/limine/limine-install _qemu_img
	BYPRODUCTS _qemu_img
	VERBATIM
)

add_custom_target( run
	DEPENDS img
	COMMAND qemu-system-x86_64 -cdrom _qemu_img -debugcon stdio -m 1G
	VERBATIM
)

include_directories( . )

set( TOOLCHAIN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/toolchain" )
set( TOOLCHAIN_BIN_PATH "${TOOLCHAIN_ROOT}/Local/bin" )
set( TOOLCHAIN_BIN_PREFIX "${TOOLCHAIN_BIN_PATH}/${AXIOM_ARCH}-pc-elf" )

set( CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PREFIX}-gcc )
set( CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PREFIX}-g++ )
set( CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_PREFIX}-gcc )
set( CMAKE_LINKER ${TOOLCHAIN_BIN_PREFIX}-ld )
set( CMAKE_RANLIB ${TOOLCHAIN_BIN_PREFIX}-gcc-ranlib )
set( CMAKE_STRIP ${TOOLCHAIN_BIN_PREFIX}-strip )
set( CMAKE_AR ${TOOLCHAIN_BIN_PREFIX}-gcc-ar )
set( CMAKE_OBJCOPY ${TOOLCHAIN_BIN_PREFIX}-objcopy )

foreach( lang ASM C CXX OBJC OBJCXX )
	unset( CMAKE_${lang}_OSX_COMPATIBILITY_VERSION_FLAG )
	unset( CMAKE_${lang}_OSX_CURRENT_VERSION_FLAG )
	unset( CMAKE_${lang}_LINK_FLAGS )
	unset( CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS )
	unset( CMAKE_SHARED_MODULE_CREATE_${lang}_FLAGS )
	unset( CMAKE_SHARED_MODULE_LOADER_${lang}_FLAG )
	unset( CMAKE_${lang}_OSX_DEPLOYMENT_TARGET_FLAG )
	unset( CMAKE_${lang}_SYSROOT_FLAG )
	if ( CMAKE_SYSTEM_NAME MATCHES Darwin )
		## macOS workaround. Use GNU ld flags for SONAMEs.
		set( CMAKE_${lang}_CREATE_SHARED_LIBRARY
			"<CMAKE_${lang}_COMPILER> <LANGUAGE_COMPILE_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS> <LINK_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>" )
		set( CMAKE_SHARED_LIBRARY_SONAME_${lang}_FLAG "-Wl,-soname," )
	endif()
endforeach()

add_subdirectory( kernel )
