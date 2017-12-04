import msgs

class ComMessage(msgs.Message):
    def __init__(self, com):
        #TODO: Right now we will only support solid colors, need to change this later
        self.com = com
        self.type = MsgType.DATA

    def serialize(self):
        return "{}:{}".format(self.type.name, str(self.com))

    @staticmethod
    def deserialize(str):
        if str.startswith("DATA:"):
            components = (str[len("DATA:"):]).split(,)
            com = str(compentents[0])
            return ComMessage(com)
        else:
            raise ValueError