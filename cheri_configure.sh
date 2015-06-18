#!/bin/sh
env SDK=/home/mbv21/ctsrd/cherilibs/trunk/tools/sdk./configure --cc=$SDK/bin/clang --enable-cross-compile --arch=mips --target-os=freebsd --nm=$SDK/bin/nm --disable-asm --disable-shared --enable-static --extra-cflags="-mabi=sandbox -msoft-float" --disable-libdcadec --host-cc=$SDK/bin/clang
