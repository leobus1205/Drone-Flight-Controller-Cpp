#!/bin/sh
sudo raspi-config nonint do_update
sudo raspi-config nonint do_expand_rootfs 1
sudo raspi-config nonint do_change_timezone Asia/Tokyo
sudo raspi-config nonint do_change_locale ja_JP.UTF-8
sudo raspi-config nonint do_ssh 1
sudo raspi-config nonint do_spi 1
sudo raspi-config nonint do_i2c 1
sudo apt -y update
sudo apt -y upgrade
sudo apt -y dist-upgrade
sudo rpi-update -y
sudo raspi-config nonint do_configure_keyboard
echo "Setup finished"
sudo reboot