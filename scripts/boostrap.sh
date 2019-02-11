#!/usr/bin/env bash

set -e

sudo apt-get update

sudo apt-get install -y libcurl4-gnutls-dev

mkdir -p ./vendor/
cd ./vendor/


if [[ ! -d htmlcxx ]]; then
    wget https://netix.dl.sourceforge.net/project/htmlcxx/htmlcxx/0.86/htmlcxx-0.86.tar.gz
    tar xvzf htmlcxx-0.86.tar.gz
    rm -f htmlcxx-0.86.tar.gz
    mv htmlcxx-0.86 htmlcxx

    cd htmlcxx
    ./configure
    make
    sudo make install
    sudo ldconfig
    cd .. # goto vendor dir
fi



sudo apt-get -y install flex
if [[ ! -d hcxselect ]]; then
    wget https://master.dl.sourceforge.net/project/hcxselect/hcxselect-1.1/hcxselect-1.1.tar.gz
    tar xvzf hcxselect-1.1.tar.gz
    rm -f hcxselect-1.1.tar.gz
    mv hcxselect-1.1 hcxselect
    cd hcxselect
    make src
    cd .. # goto vendor dir
fi


cd .. # goto project dir


mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
make
sudo make install
