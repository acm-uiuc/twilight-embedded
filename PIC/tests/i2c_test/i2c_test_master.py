#!/usr/bin/python

import RPi.GPIO as GPIO
import smbus

slaveAddress = 0x12
DEVICE_REG_LEDOUT0= 0x09

def readMessageFromArduino():
    global smsMessage
    data_received_from_Arduino = i2c.read_i2c_block_data(slaveAddress, 0,15)
    print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))
    for i in range(len(data_received_from_Arduino)):
        smsMessage += chr(data_received_from_Arduino[i])

    print(smsMessage.encode('utf-8'))
    data_received_from_Arduino =""
    smsMessage = ""

smsMessage = ""
smsNumber = ""

if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)

    i2c = smbus.SMBus(1)

    while 1:
        try:
           
            #readMessageFromArduino()
            ledout_values = [0x33, 0x33, 0x33, 0x33, 0x33, 0x33]
            i2c.write_i2c_block_data(slaveAddress, 0x03, ledout_values)

            try:
                readMessageFromArduino()
            except IOError:
                pass
        except KeyboardInterrupt:
               GPIO.cleanup()