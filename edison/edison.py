import json
import os
import re
from subprocess import Popen, PIPE,call

call(["rfkill", "unblock", "bluetooth"])
call(["hciconfig", "hci0", "up"])

process = Popen(["sdptool", "del", "HomeAutoService"], stdout=PIPE)
(output, err) = process.communicate()
exit_code = process.wait()

process = Popen(["sdptool", "add", "--handle=HomeAutoService","--channel=19"], s
(output, err) = process.communicate()
exit_code = process.wait()
print output

process = Popen(["rfcomm", "listen", "hci0","1"], stdout=PIPE)
while True:
        line = process.stdout.readline()
        if line != '':
                print "test:", line.rstrip()
        else:
                break


'''
process = Popen(["hcitool", "scan"], stdout=PIPE)
(output, err) = process.communicate()
exit_code = process.wait()
print output


process = Popen(["hcitool", "scan"], stdout=PIPE)
(output, err) = process.communicate()
exit_code = process.wait()
print output

ids = []
for line in output.split('\n'):
        parts = line.split()
        if len(parts)>0 and not ('Scanning' in parts):
                ids.append(parts[0])
print "Found IDs:%s"%str(ids)

goodId = None
for id in ids:
        process = Popen(["sdptool", "browse",str(id)], stdout=PIPE)
        (output, err) = process.communicate()
        exit_code = process.wait()

        services = output.split('\n\n')
        for service in services:
                if 'HomeAutomationService' in service:
                        lines = service.split('\n')
                        line = [line for line in lines if 'channel' in line][0]
                        goodId = id
if goodId!=None:
        channel = line.split('=')[1]

        call(["sdptool", "add", "--channel="+str(channel),'SP'])
        process = Popen(["rfcomm", "connect", "hci0", str(goodId),str(channel)],
        while True:
                line = proc.stdout.readline()
                if line != '':
                        print "test:", line.rstrip()
                else:
                        break
'''
