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
        commands.getoutput('hciconfig hci0 down') ##how to here?
        commands.getoutput('hciconfig hci0 up')
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

                contained = False
		for existing_match in matches:
			if device[0] == existing_match[0]:
				contained = True
				break

		if not contained:
	       		matches.append(device)

        return matches

if __name__ == "__main__":
	btController = BtController()
	print btController.discover()
