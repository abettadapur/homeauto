import abc
from utils import consts
from model.module import ModuleBase

class FlipModule(ModuleBase):

    def __init__(self, id, address, socket):
        super(ModuleBase, self).__init__(id, address, consts.FLIP_MODULE_TYPE, socket)

    def action(self, **kwargs):
        pass

    def status(self):
        pass
