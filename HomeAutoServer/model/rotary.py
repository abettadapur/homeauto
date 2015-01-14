import abc
from flask_restful.representations import json
from utils import consts
from model.module import ModuleBase

class RotaryModule(ModuleBase):

    def __init__(self, id, address, peripheral):
        super(ModuleBase, self).__init__(id, address, consts.ROTARY_MODULE_TYPE, peripheral)

    def action(self, **kwargs):
        #get service, write to characteristic
        pass

    def status(self):
        # get service read from characteristic
        pass

