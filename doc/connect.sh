#!/bin/bash
if [ ! -f ~/.minirc.dfl ]
  then
    cp minirc.dfl ~/.minirc.dfl
fi
MINICOM='-c on -b 38400 -D /dev/ttyUSB0'
export MINICOM
minicom
