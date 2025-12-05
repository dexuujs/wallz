# ILYWallz v0.01 [prototype]

abc 🔥🔥🔥

## how to use this?

### dependencies

any `make` command depends on GNU make (`gmake`) and is expected to be run using it, this usually means using `make` on most GNU/Linux distros or `gmake` on other non-GNU systems

all `make all*` targets depend on a GNU-compatible C toolchain capable of generating x86-64 ELF objects, most of the time any `gcc/binutils` or `clang/llvm/lld` provided by any x86-64 UNIX like (including Linux) distribution will suffice.

also building an ISO with `make all` requires `xorriso` and building a HDD/USB image with `make all-hdd` requires `sgdisk` (usually from `gdisk` or `gptfdisk` packages) and `mtools`

### toolchain selection

`TOOLCHAIN` and `TOOLCHAIN_PREFIX` `make` variables can be used to set the toolchain, `TOOLCHAIN` can be set to `llvm` to use Clang/LLVM

for example:
```bash
make TOOLCHAIN=llvm
```
or:
```bash
make TOOLCHAIN_PREFIX=x86_64-elf-
```

### makefile targets

* running `make all` will compile the kernel and then generate a bootable ISO image
* running `make all-hdd` will compile the kernel and then generate a raw image suitable to be flashed onto a USB stick or hard drive/SSD
* running `make run` will build the kernel and a bootable ISO and then run it using `qemu`
* running `make run-hdd` will build the kernel and a raw HDD image and then run it using `qemu`

the `run-uefi` and `run-hdd-uefi` targets are equivalent to their non `-uefi` counterparts except that they boot `qemu` using a UEFI-compatible firmware