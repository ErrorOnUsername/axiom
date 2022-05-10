<p align="center">
	<img width=400 src="meta/logo.png"/>
</p>

---

## To-Do List
- [ ] apic/ioapic
- [ ] hpet
- [ ] scheduler
- [ ] syscall interface
- [ ] IPC

---

## Build Requirements

You'll need `cmake` `make` `gcc` `xorriso` `qemu` and `mtools` (TODO: verify)

## Building Axiom
  * Clone the repo ***recursively*** (e.g. `git clone --recusive https://github.com/ErrorOnUsername/axiom.git`
  * Build the toolchain
    - `cd` into the Toolchain directory
    - run `./build_toolchain.sh` (make sure that `build_toolchain.sh` has the executable (x) property
  * Build the kernel and iso
    - create a build directory in the root of the project and `cd` into it
    - run `cmake ..`
    - run `make && make run`
