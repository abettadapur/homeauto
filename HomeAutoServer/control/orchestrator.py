from model.module import ModuleBase
from bt.btcontroller import BluetoothServer

class Orchestrator(object):
    modules = []

    def __init__(self):
        pass

    def get_modules(self):
        return self.modules.__dict__
