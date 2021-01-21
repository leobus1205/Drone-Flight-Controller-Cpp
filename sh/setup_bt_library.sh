#!/bin/sh
echo "Setup bluetooth library"
sudo apt-get install bluez
sudo apt-get install bluetooth blueman bluez-tool
sudo apt-get install libbluetooth-dev
sudo chmod 777 /etc/systemd/system/dbus-org.bluez.service
sudo echo "ExecStart=/usr/lib/bluetooth/bluetoothd —-compat" >> /etc/systemd/system/dbus-org.bluez.service
sudo echo "ExecStartPost=/usr/bin/sdptool add SP" >> /etc/systemd/system/dbus-org.bluez.service
echo "Setup finished"
sudo reboot