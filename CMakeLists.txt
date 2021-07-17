cmake_minimum_required( VERSION 3.13 )
project( axiom CXX )

set( CMAKE_CXX_STANDARD 20 )

# TODO: Make a toolchain!
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
set( CMAKE_CXX_COMPILER gcc-11 )
set( CMAKE_LINKER ld )

include_directories( . )

add_subdirectory( kernel )

add_custom_target( img
	COMMAND make -C ../limine
	COMMAND mkdir -p iso_root
	COMMAND cp -v kernel/kernel ../limine.cfg ../limine/limine.sys ../limine/limine-cd.bin
	           ../limine/limine-eltorito-efi.bin iso_root
	COMMAND xorriso -as mkisofs -b limine-cd.bin
	                -no-emul-boot -boot-load-size 4 -boot-info-table
	                --efi-boot limine-eltoito-efi.bin
	                -efi-boot-part --efi-boot-image --protective-msdos-label
					iso_root -o _qemu_img
	COMMAND ./../limine/limine-install _qemu_img
	BYPRODUCTS _qemu_img
	VERBATIM
)

add_custom_target( run
	COMMAND qemu-system-x86_64 -cdrom _qemu_img
	                           -serial stdio
	                           -m 1024M
	VERBATIM
)
