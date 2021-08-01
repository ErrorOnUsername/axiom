cmake_minimum_required( VERSION 3.13 )
project( axiom CXX )

set( CMAKE_CXX_STANDARD 20 )

# TODO: Make a toolchain!
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

include_directories( . )

add_subdirectory( Kernel )

add_custom_target( img
	COMMAND make -C ../Vendor/limine
	COMMAND mkdir -p iso_root
	COMMAND cp -v Kernel/Kernel ../limine.cfg ../Vendor/limine/limine.sys ../Vendor/limine/limine-cd.bin
	           ../Vendor/limine/limine-eltorito-efi.bin iso_root
	COMMAND xorriso -as mkisofs -b limine-cd.bin
	                -no-emul-boot -boot-load-size 4 -boot-info-table
	                --efi-boot limine-eltorito-efi.bin
	                -efi-boot-part --efi-boot-image --protective-msdos-label
					iso_root -o _qemu_img
	COMMAND ./../Vendor/limine/limine-install _qemu_img
	BYPRODUCTS _qemu_img
	VERBATIM
)

add_custom_target( run
	COMMAND qemu-system-x86_64 -cdrom _qemu_img
	                           -serial stdio
	                           -m 1024M
	VERBATIM
)
