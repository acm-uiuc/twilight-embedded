import smbus

i2c = smbus.SMBus(1)
slave = 0x12

CMD = "LED:255,0,255"

for b in CMD:
    i2c.write_byte(slave, ord(b))

data_received_from_Arduino = i2c.read_i2c_block_data(slave, 0,16)
print '[{}]'.format(', '.join(hex(x) for x in data_received_from_Arduino))