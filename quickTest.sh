#!/bin/zsh

./build/src/tipc -do -asm ../tests/siptest.sip
/usr/local/opt/llvm/bin/llc ../tests/siptest.sip.ll
