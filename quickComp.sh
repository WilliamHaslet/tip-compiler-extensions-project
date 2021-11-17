#!/bin/zsh

make -C build -j6
find . -name '*gcda' -delete
