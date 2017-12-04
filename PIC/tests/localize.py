
import smbus
from time import sleep
import argparse

PARSER = argparse.ArgumentParser(description=None)

PARSER.add_argument("cmd", type=str)


ARGS = PARSER.parse_args()

i2c = smbus.SMBus(1)
slave = 0x12

CMD = ARGS.cmd

if ARGS.cnd =="LOC":
    CMD = "LOC:0,0"

LOC = (0,0)

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

vis = lambda x: (100 + (x*50)) % 255

def display_loc():
    send_str("LED:"+vis(LOC[0])+','+vis(LOC[1])+','+'0')


if CMD == "read":
    count = 0 
    while 1:
        if count % 500000 == 0:
            LED_CMD = read()
            print(LED_CMD)
            if LED_CMD.startswith("COM:"):
                SUB_CMD = LED_CMD.split("COM:")[1]
                if SUB_CMD.startswith("LED:"):
                    send_str(SUB_CMD)
            elif LED_CMD.startswith("LOC:"):
                LOC_CMD = LOC_CMD.split("LOC:")[1]
                INCOMMING_LOC = LOC_CMD.split(";")
                ASSIGNER_LOC = tuple(INCOMMING_LOC[0].split(","))
                print ("ASSIGNER LOC:{}", ASSIGNER_LOC)
                if  INCOMMING_LOC[1] == 0:
                    LOC = (ASSIGNER_LOC[0] + 1, ASSIGNER_LOC[1])
                elif INCOMMING_LOC[1] == 1:
                    LOC = (ASSIGNER_LOC[0] - 1, ASSIGNER_LOC[1])
                elif INCOMMING_LOC[1] == 2: 
                    LOC = (ASSIGNER_LOC[0], ASSIGNER_LOC[1] - 1)
                elif INCOMMING_LOC[1] == 3:
                    LOC = (ASSIGNER_LOC[0],ASSIGNER_LOC[1] + 1)
                print ("ASSIGNED LOC:{}", LOC)
                display_loc()
        count += 1
else:
    send_str(CMD)
    sleep(0.5)
    if CMD.startswith("COM:"):
        LED_CMD = CMD.split("COM:")[1]
        if LED_CMD .startswith("LED:"):
            send_str(LED_CMD )
    elif CMD.startswith("LOC:"):
        display_loc()
    

