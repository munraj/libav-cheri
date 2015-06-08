#!/bin/sh
./configure --cc=/home/mbv21/ctsrd/cherilibs/trunk/tools/sdk/bin/clang --enable-cross-compile --arch=mips --target-os=freebsd --nm=/home/mbv21/trunk/tools/sdk/bin/nm --disable-asm --disable-shared --enable-static --extra-cflags="-mabi=sandbox -msoft-float" --disable-libdcadec
