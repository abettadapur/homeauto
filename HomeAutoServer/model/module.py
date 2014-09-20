import abc

class ModuleBase(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, id, address, type, socket):
        self.id = id
        self.address = address
        self.type = type
        self.client_socket = socket
    @abc.abstractmethod
    def action(self, **kwargs):
        raise NotImplementedError

    @abc.abstractmethod
    def status(self):
        raise NotImplementedError