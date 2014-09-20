import abc
from utils import consts
from model.module import ModuleBase

class FlipModule(ModuleBase):

    def __init__(self, id, address):
        super(ModuleBase, self).__init__(id, address, consts.FLIP_MODULE_TYPE)

    def action(self, **kwargs):
        pass

    def status(self):
        pass
