import abc
from enum import Enum
import localization
import led
import data
import cmd

class MessageType(Enum):
    LOC = 1
    LED = 2
    COM = 3
    CMD = 4

class Source(Enum):
    WEST  = 0
    EAST  = 1
    NORTH = 2
    SOUTH = 3

    SELF  = 5

class Message(abc.ABC):
    @abc.abstractmethod
    def __init__(self):
        '''
        Instanciate a message
        '''
        pass

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        return

    @abc.abstractmethod
    def serialize(self):
        pass

    @staticmethod
    def deserialize(str, source=None):
        '''
        deserialize a string for use in an application
        Format:
        [MSG TYPE]:[CONTENT]
        '''
        if str.startswith("LED:"):
            return led.LEDMessage.deserialize(str)
        elif str.startswith("LOC:"):
            return localization.LocalizationMessage.deserialize(str, source)
        elif str.startswith("COM:"):
            return data.ComMessage.deserialize(str)
        else:
            raise ValueError
