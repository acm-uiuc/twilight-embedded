#!/bin/bash
# USAGE: setup_device.sh [device_id]
sudo rm /etc/hosts
sudo cp hosts.template  /etc/hosts
sudo echo -e "127.0.1.1	        twilight_$1" >> /etc/hosts
sudo echo -e "twilight_$1" > /etc/hostname

sudo apt update
sudo apt install -y avrdude python-smbus i2c-tools
avrdude -v
cat /etc/avrdude.conf avr_programming_pins.conf > ~/avrdude_gpio.conf
echo -e "Setup done, make sure to enable I2C with raspi-config and set the fuses with set_fuses and reboot"