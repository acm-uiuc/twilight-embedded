import smbus
from time import sleep
import argparse
from flask import Flask

app = Flask(__name__)

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


@app.route('/color', methods=['GET'])
def show_post():
    r = request.args.get('r')
    b = request.args.get('g')
    g = request.args.get('b')

    return 


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
        count += 1
else:
    app = Flask(__name__)
    @app.route('/color', methods=['GET'])
    def show_post():
        r = request.args.get('r')
        b = request.args.get('g')
        g = request.args.get('b')

        CMD = "COM:LED:" + r + g + b

        send_str(CMD)
        sleep(0.5)
        if CMD.startswith("COM:"):
            LED_CMD = CMD.split("COM:")[1]
            if LED_CMD .startswith("LED:"):
                send_str(LED_CMD )


