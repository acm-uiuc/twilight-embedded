import RPi.GPIO as GPIO
import smbus
from protocols import messages

PIC_ADDR = 0x12 #I2C Address of the PIC

class MessageBus():
    def __init__(self):
        self.inbox = []
        self.outbox = []
        self.i2c = smbus.SMBus(1)

    def check_for_msgs(self):
        try:
            data_size = self.i2c.read_i2c_block_data(PIC_ADDR, 0,1)
            data_recv = self.i2c.read_i2c_block_data(PIC_ADDR, data_size,15)
            for i in range(len(data_recv)):
                if ((data_recv[i] >= 0x20) and (data_recv[i] <= 0x7f)):
                    data_recv += chr(data_recv[i])
            _msg = data_recv.split(';')
            msg_struct = messages.deserialize(_msg[0],_msg[1])
            self.inbox.append(msg_struct)
        except IOError:
            pass
        return

    def write():
        for m in self.outbox:
            '''protocol for sending msgs over i2c'''
            data = m.serialize()
            for b in data:
                self.i2c.write_byte(PIC_ADDR, b)
        return 

    def send_msg(self, msg):
        self.outbox.append(msg.serialize())
        return

    def recv_msgs(self):
        incomming_msgs = self.inbox
        self.inbox = []
        return incomming_msgs