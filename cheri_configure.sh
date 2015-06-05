#!/bin/sh
./configure --cc=/home/mbv21/trunk/tools/sdk/bin/clang --enable-cross-compile --arch=mips --target-os=freebsd --nm=/home/mbv21/trunk/tools/sdk/bin/nm --disable-asm --disable-shared --enable-static --extra-cflags="-msoft-float"
