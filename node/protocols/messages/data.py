import msgs

class DataMessage(msgs.Message):
    def __init__(self, data):
        #TODO: Right now we will only support solid colors, need to change this later 
        self.data = data
        self.type = MsgType.DATA

    def serialize(self):
        return "{}:{}".format(self.type.name, str(self.data))

    @staticmethod
    def deserialize(str):
        if str.startswith("DATA:"):
            components = (str[len("DATA:"):]).split(,)
            data = str(compentents[0])
            return DataMessage(data)
        else:
            raise ValueError