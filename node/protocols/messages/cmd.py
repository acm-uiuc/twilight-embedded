import msgs

class CmdMessage(msgs.Message):
    def __init__(self, cmd):
        #TODO: Right now we will only support solid colors, need to change this later 
        self.data = cmd
        self.type = MsgType.CMD

    def serialize(self):
        return "{}:{}".format(self.type.name, str(self.CMD))

    @staticmethod
    def deserialize(str):
        if str.startswith("CMD:"):
            components = (str[len("CMD:"):]).split(,)
            cmd = str(compentents[0])
            return CmdMessage(cmd)
        else:
            raise ValueError