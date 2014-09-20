import abc
import json
from utils import consts
from model.module import ModuleBase

class FlipModule(ModuleBase):

    def __init__(self, id, address, socket):
        super(ModuleBase, self).__init__(id, address, consts.FLIP_MODULE_TYPE, socket)

    def action(self, **kwargs):
        self.client_socket.send('{"command": "action", "params": {0}'.format(json.dumps(**kwargs)))

    def status(self):
        self.client_socket.send('{"command": "status", "params": {}')
        data = self.client_socket.recv(1024)
        return data
