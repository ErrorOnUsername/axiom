command_fail() {
	echo "$1 FAILURE! EXITING..."
	exit $?
}

mkdir -p Build/iso/boot/grub
cd Build

cmake ..
if [ ! $? -eq 0 ]; then
	command_fail "CMAKE"
fi

make
if [ ! $? -eq 0 ]; then
	command_fail "MAKE"
fi

cp ../grub.cfg iso/boot/grub
if [ ! $? -eq 0 ]; then
	command_fail "COPY"
fi

cp Kernel/Kernel iso/boot
if [ ! $? -eq 0 ]; then
	command_fail "COPY"
fi

grub-mkrescue -o _axiom_qemu iso
if [ ! $? -eq 0 ]; then
	command_fail "GRUB-MKRESCUE"
fi

qemu-system-x86_64 -cdrom _axiom_qemu -serial stdio -m 1024M
if [ ! $? -eq 0 ]; then
	command_fail "QEMU"
fi

cd ..
