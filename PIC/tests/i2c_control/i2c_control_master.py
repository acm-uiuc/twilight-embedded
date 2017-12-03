import smbus
import argparse

PARSER = argparse.ArgumentParser(description=None)

PARSER.add_argument('r', type=str)
PARSER.add_argument('g', type=str)
PARSER.add_argument('b', type=str)

ARGS = PARSER.parse_args()



i2c = smbus.SMBus(1)
slave = 0x12

CMD = "LED:"+ARGS["r"]+ARGS["g"]+ARGS["b"]

for b in CMD:
    i2c.write_byte(slave, ord(b))

data_received_from_Arduino = i2c.read_i2c_block_data(slave, 0,16)
print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))
