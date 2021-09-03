# Axiom

A 64-bit OS that I'm making for fun :)

## Build Requirements

You'll need `cmake` `make` `gcc` `xorriso` `qemu` and `mtools`

### On Arch Systems:
`sudo pacman -S cmake make gcc xorriso qemu mtools`

## Building Axiom
  * Clone the repo ***recursively*** (e.g. `git clone --recusive https://github.com/ErrorOnUsername/axiom.git`
  * Build the toolchain
    - `cd` into the Toolchain directory
    - run `./build_toolchain.sh` (make sure that `build_toolchain.sh` has the executable (x) property
  * Build the kernel and iso
    - create a build directory in the root of the project and `cd` into it
    - run `cmake ..`
    - run `make && make run`
