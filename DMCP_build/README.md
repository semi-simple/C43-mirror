These scripts are meant for Linux only.

You need to have a working arm-none-eabi tool chain installed on your system and configured in your PATH variable.
You can download such a tool chain here: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads or may be you can install it with your distro's package manager.

The compileGMP script download the gmplib sources and compiles a static library for ARM. This script should only be run once. Only 2 files are used for the next step: gmp.h and libgmp.a

The compileDMCP script builds the file WP43S.pgm in the build directory. This file must be copied to the DM42's FAT file system with a USB cable.
