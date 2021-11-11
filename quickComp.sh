#!/bin/zsh

make -C build -j6
find . -name '*gcda' -delete
./build/src/tipc -do -asm ../tests/siptest.sip
/usr/local/opt/llvm/bin/llc ../tests/siptest.sip.ll
