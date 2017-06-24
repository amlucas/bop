#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop

s = bop_pop()
s = bop_pop()
bop_push()
s = bop_pop()
s = bop_pop()
s = bop_pop()

