#!/bin/bash
gcc -o vi vi.c -lncurses -lreadline
gcc -o thread thread.c -lncurses -lreadline
./thread