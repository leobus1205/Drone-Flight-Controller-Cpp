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
#wget https://project-downloads.drogon.net/wiringpi-latest.deb
#sudo dpkg -i wiringpi-latest.deb
cd include
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
cd ../../
echo "Create dir cmake"
mkdir ../cmake
echo "Get Download_Project.cmake"
git clone https://github.com/Crascit/DownloadProject.git ../cmake/DownloadProject
echo "Create dir build"
mkdir ../build

