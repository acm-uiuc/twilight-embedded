import smbus
import argparse

PARSER = argparse.ArgumentParser(description=None)

PARSER.add_argument("cmd", type=str)


ARGS = PARSER.parse_args()



i2c = smbus.SMBus(1)
slave = 0x12

CMD = ARGS.cmd

def send_str(str): 
    i2c.write_byte(slave, 2)
    for b in str:
        i2c.write_byte(slave, ord(b))
    i2c.write_byte(slave, 3)

def read(): 
    data_received_from_Arduino = i2c.read_i2c_block_data(slave, 0,16)
    print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))

if CMD == "read":
    read()
else:
    send_str(CMD)


