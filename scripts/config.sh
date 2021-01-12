#!/bin/sh

# Install cxx compiler
sudo apt install build-essential

# Install dependencies
sudo apt install libsfml-dev libwebkit2gtk-4.0-dev libomp-dev libpython3-all-dev

# Install scons for build automation
sudo apt install scons

# Boost download and build from source
mkdir dl
cd dl/
wget https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz
tar -xf boost_1_75_0.tar.gz
cd boost_1_75_0/
./bootstrap.sh
./b2 tools/bcp
mkdir ../boostbuild
./dist/bin/bcp build interprocess test lockfree json dll ../boostbuild
cd ../boostbuild
./bootstrap.sh
./b2
cd ../..
mkdir lib
mv dl/boostbuild/ lib/boost
rm dl/ -R

# Webview download
cd lib 
git clone https://github.com/webview/webview.git
cd ..