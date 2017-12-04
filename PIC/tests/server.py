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

    return 


