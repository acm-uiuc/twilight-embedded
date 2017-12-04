
import smbus
import argparse

PARSER = argparse.ArgumentParser(description=None)

PARSER.add_argument("cmd", type=str)


ARGS = PARSER.parse_args()



i2c = smbus.SMBus(1)
slave = 0x12

CMD = ARGS.cmd

def send_str(str):
    data = tuple(map(lambda x: ord(x), str)) + (3,)
    i2c.write_i2c_block_data(slave, 2, list(data))


def read(): 
    try:
        data_received_from_Arduino = i2c.read_i2c_block_data(slave, 0,32)
        print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))
        str_in = "" 
        for c in data_received_from_Arduino:
            if c != 0xFF:
                str_in += chr(c)
        return str_in
    except:
        return "IO ERROR"


if CMD == "read":
    count = 0 
    while 1:
        if count % 500000 == 0:
            LED_CMD = read()
            print(LED_CMD)
            if LED_CMD.startswith("COM:"):
                SUB_CMD = LED_CMD.split("COM:")[1]
                if SUB_CMD.startswith("LED:"):
                    send_str(CMD)
        count += 1
else:
    if CMD.startswith("COM:"):
        LED_CMD = CMD.split("COM:")[1]
        if LED_CMD .startswith("LED:"):
            send_str(LED_CMD )
    send_str(CMD)








'''
import smbus
from random import randint
import platform
NAME = platform.node()

i2c = smbus.SMBus(1)
slave = 0x12

def rand_px_num():
    return str(randint(0, 255))

def send_str(str): 
    i2c.write_byte(slave, 2)
    for b in str:
        i2c.write_byte(slave, ord(b))
    i2c.write_byte(slave, 3)

def read(): 
    data_received_from_Arduino = i2c.read_i2c_block_data(slave, 0,16)
    print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))
    str_in = "" 
    for c in data_received_from_Arduino:
        if c != 0xFF:
            str_in += chr(c)
    return str_in

def main():
    count = 0 
    while 1:
        if count % 50000 == 0 and NAME == 'twilight_6': 
            LED_CMD = read() 
            if LED_CMD.startswith("COM:"):
                CMD = LED_CMD.split("COM:")[1]
                send_str(CMD)
        elif count % 100000 == 0 and NAME == 'twilight_5':
            send_str("COM:LED:" + rand_px_num() + ','  + rand_px_num() + ',' + rand_px_num())
        count += 1

if __name__ == "__main__":
    main()
'''