#!/bin/bash
gcc -o vi vi.c -lncurses -lreadline
gcc -o D D_createVI.c -lncurses -lreadline
./D