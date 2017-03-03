#!/bin/bash

sudo apt-get install software-properties-common
sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
sudo add-apt-repository --yes ppa:george-edison55/precise-backports
sudo apt-get update
sudo apt-get install g++-4.8 lcov -y
sudo apt-get remove cmake -y
sudo apt-get install cmake -y
