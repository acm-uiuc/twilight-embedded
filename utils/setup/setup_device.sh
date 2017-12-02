#!/bin/bash
# USAGE: setup_device.sh [device_id]
if [[ $UID != 0 ]]; then
    echo "Please run this script with sudo:"
    echo "sudo $0 $*"
    exit 1
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
sudo rm /etc/hosts
sudo cp $DIR/hosts.template  /etc/hosts
sudo echo -e "127.0.1.1	      twilight_$1" >> /etc/hosts
sudo echo -e "twilight_$1" > /etc/hostname

sudo apt update
sudo apt install -y avrdude python-smbus i2c-tools
avrdude -v
cat /etc/avrdude.conf $DIR/avr_programming_pins.conf > $DIR/../avrdude_gpio.conf
echo -e "Setup done, make sure to enable I2C with raspi-config and set the fuses with set_fuses and reboot"