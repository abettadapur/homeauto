from uuid import uuid4
from model.flip import FlipModule
from model.module import ModuleBase
from model.poke import PokeModule
from model.rotary import RotaryModule
from utils import consts
from bt.btcontroller import BtController

class Orchestrator(object):


    def __init__(self):
        self.modules = {}
        self.bt_controller = BtController()

    def get_modules(self):
        return self.modules.__dict__

    def module_action(self, module_id, **kwargs):
        module = self.modules[module_id]
        module.action(kwargs)

    def module_status(self, module_id):
        return self.modules[module_id].status()

    #TODO(abettadapur): Need some database to store modules on device shutdown
    def add_module(self, address, type):
        id = uuid4()
        if type == consts.FLIP_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = flip.Flip(id, address, consts.FLIP_MODULE_TYPE, peripheral)
            self.modules[id] = module

        elif type == consts.POKE_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = poke.Poke(id, address, consts.POKE_MODULE_TYPE, peripheral)
            self.modules[id] = module

        elif type == consts.ROTARY_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = rotary.Rotary(id, address, consts.ROTARY_MODULE_TYPE, peripheral)
            self.modules[id] = modules

    def scan_modules(self):
        modules = self.bt_controller.discover()

    def remove_module(self, id):
        del self.modules[id]
