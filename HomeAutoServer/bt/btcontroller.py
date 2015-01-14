import os
import signal
import subprocess
import threading
import sys
import time
import commands
import re
import logging
from utils import consts
from model import *


from btle import Peripheral, Service, Characteristic, UUID


class BtController(object):

    def __init__(self):
        self.modules = {}
        logging.basicConfig()

    def discover(self):
        pattern = re.compile('^(\w\w:\w\w:\w\w:\w\w:\w\w:\w\w)(.+)')
        print commands.getoutput("hcitool dev")
        commands.getoutput('hciconfig hci1 down') ##how to here?
        commands.getoutput('hciconfig hci1 up')
        print commands.getoutput("hcitool dev")
        commands.getoutput('killall hcitool')

        p = subprocess.Popen('hcitool lescan', bufsize = 0,shell = True, stdout =subprocess.PIPE,stderr = subprocess.STDOUT)
        time.sleep(3)

        matches = []
        for i in range(0,30,1):
            inchar = p.stdout.readline()
            match = pattern.match(inchar)
            if match is not None:
                device = (match.group(1), match.group(2))
                if not device in matches:
                    matches.append(device)

        return matches

    def serve(self):
        pass

    #TODO(abettadapur): Need some database to store modules on device shutdown

    def add_module(self, address, type):
        if type == consts.FLIP_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = flip.Flip(0x0, address, consts.FLIP_MODULE_TYPE, peripheral)
            self.modules[id] = module

        elif type == consts.POKE_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = poke.Poke(0x0, address, consts.POKE_MODULE_TYPE, peripheral)
            self.modules[id] = module

        elif type == consts.ROTARY_MODULE_TYPE:
            peripheral = Peripheral(address)
            module = rotary.Rotary(0x0, address, consts.ROTARY_MODULE_TYPE, peripheral)
            self.modules[id] = modules


    def action(self, id):
        self.modules[id].action()

    def status(self, id):
        return self.modules[id].status()
