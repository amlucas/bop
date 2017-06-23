#!/usr/bin/env octave-qf

global e_c e_m
system("make >/dev/null");
e_c = 0;
pkg load bop
B = bop_read("test_data/1000.bop");

bop_write(B, "./123.bop");
