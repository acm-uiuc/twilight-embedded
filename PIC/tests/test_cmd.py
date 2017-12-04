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