from uuid import uuid4
from model.flip import FlipModule
from model.module import ModuleBase
from model.poke import PokeModule
from model.rotary import RotaryModule
from utils import consts
from bt.btcontroller import BluetoothServer

class Orchestrator(object):


    def __init__(self):
        self.modules = {}

    def get_modules(self):
        return self.modules.__dict__

    def register_module(self, socket, socket_info, module_info):
        if module_info['type'] == consts.FLIP_MODULE_TYPE:
            new_module = FlipModule(uuid4(), socket_info, socket)
        elif module_info['type'] == consts.POKE_MODULE_TYPE:
            new_module = PokeModule(uuid4(), socket_info, socket)
        elif module_info['type'] == consts.ROTARY_MODULE_TYPE:
            new_module = RotaryModule(uuid4(), socket_info, socket)

        self.modules[new_module.id] = new_module

    def module_action(self, module_id, **kwargs):
        module = self.modules[module_id]
        module.action(kwargs)

    def module_status(self, module_id):
        return self.modules[module_id].status()
