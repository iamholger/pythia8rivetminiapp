#!/bin/bash

RBD=/home/hschulz/src/Rivet3/local311


g++ -g -O3 -std=c++14  miniapp.cxx  -o miniapp \
    -I/usr/include/eigen3 \
    -I${RBD}/include \
    -L${RBD}/lib -lYODA -lRivet -lHepMC \
    -I/home/hschulz/src/pythia8240/include \
    -L /home/hschulz/src/pythia8240/lib -lpythia8
