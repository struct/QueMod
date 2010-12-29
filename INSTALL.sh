#!/bin/bash

## This is an installation script for QueMod. It
## will create the neccessary directories, compile
## QueMod and install it. You will need to run this
## with sudo or root

quemod_dir="/opt/quemod"
quemod_headers="/opt/quemod/quemod"
quemod_plugins="/opt/quemod/plugins"

echo "You only have to run this script once!"

if [ -d $quemod_dir ]; then
    echo "QueMod directory exists"
else
    echo "Creating /opt/quemod"
    mkdir $quemod_dir
fi

if [ -d $quemod_headers ]; then
    echo "QueMod header directory exists"
else
    echo "Creating /opt/quemod/quemod"
    mkdir $quemod_headers
fi

if [ -d $quemod_plugins ]; then
    echo "QueMod plugins directory exists"
else
    echo "Creating /opt/quemod/plugins"
    mkdir $quemod_plugins
fi

cd source/
make clean ; make && make install
cd ..

echo "INSTALL.sh is finished. Type 'quemod -h' for help!"

