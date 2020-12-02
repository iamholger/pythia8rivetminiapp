#!/bin/bash

if [[ "$1" == "install" ]]
  then
    mkdir -p dependencies
    cd dependencies
      
    wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.2.2.tar.gz
    tar -xzf HepMC3-3.2.2.tar.gz

    mkdir hepmc3-build
    cd hepmc3-build
    cmake -DHEPMC3_ENABLE_ROOTIO:BOOL=OFF -DHEPMC3_ENABLE_TEST:BOOL=OFF  \
          -DHEPMC3_INSTALL_INTERFACES:BOOL=OFF -DHEPMC3_ENABLE_PYTHON:BOOL=OFF  \
          -DHEPMC3_BUILD_STATIC_LIBS:BOOL=OFF -DHEPMC3_BUILD_DOCS:BOOL=OFF  \
          -DCMAKE_INSTALL_PREFIX=../install   \
          ../HepMC3-3.2.2
    make -j 4
    make install

    cd ..
    wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8303.tgz
    tar xzf pythia8303.tgz
    cd pythia8303
    ./configure --with-hepmc3-lib=$PWD/../install/lib64 --with-hepmc3-include=$PWD/../install/include/ --prefix=$PWD/../install
    make -j 4
    make install
else
    RBD=$PWD/dependencies/install
    g++ -g -O3  miniapp.cxx  -o miniapp -I${RBD}/include  -L${RBD}/lib  -lpythia8  -L${RBD}/lib64  -lHepMC3
fi


#mkdir -p dependencies
#cd dependencies
  
#wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.2.2.tar.gz
#tar -xzf HepMC3-3.2.2.tar.gz

#mkdir hepmc3-build
#cd hepmc3-build
#cmake -DHEPMC3_ENABLE_ROOTIO:BOOL=OFF -DHEPMC3_ENABLE_TEST:BOOL=OFF  \
      #-DHEPMC3_INSTALL_INTERFACES:BOOL=OFF -DHEPMC3_ENABLE_PYTHON:BOOL=OFF  \
      #-DHEPMC3_BUILD_STATIC_LIBS:BOOL=OFF -DHEPMC3_BUILD_DOCS:BOOL=OFF  \
      #-DCMAKE_INSTALL_PREFIX=../install   \
      #../HepMC3-3.2.2
#make -j 4
#make install

#cd ..
#wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8303.tgz
#tar xzf pythia8303.tgz
#cd pythia8303
#./configure --with-hepmc3-lib=$PWD/../install/lib64 --with-hepmc3-include=$PWD/../install/include/ --prefix=$PWD/../install
#make -j 4
#make install

#cd ../../

#RBD=$PWD/dependencies/install

#g++ -g -O3  miniapp.cxx  -o miniapp -I${RBD}/include  -L${RBD}/lib  -lpythia8  -L${RBD}/lib64  -lHepMC3
