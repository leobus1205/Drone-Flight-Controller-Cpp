#!/bin/sh
echo "Install required"
sudo apt update
sudo apt install -y build-essential
sudo apt install -y cmake
sudo apt install -y git git-core
sudo apt install -y zip unzip
sudo apt upgrade -y build-essential
sudo apt upgrade -y cmake
sudo apt upgrade -y git git-core
sudo apt upgrade -y zip unzip
wget https://project-downloads.drogon.net/wiringpi-latest.deb
sudo dpkg -i wiringpi-latest.deb
cd ../include
sudo wget https://github.com/joan2937/pigpio/archive/master.zip
sudo unzip master.zip
cd pigpio-master
sudo make
sudo make install
cd ../../
echo "Create dir cmake"
sudo mkdir cmake
echo "Get Download_Project.cmake"
sudo git clone https://github.com/Crascit/DownloadProject.git ../cmake/DownloadProject
echo "Create dir build"
sudo mkdir build

