import bluetooth
import os
from bluetooth import *
import control

class BluetoothServer(object):

    def __init__(self):
        self.connected_sockets = []
        self.server_socket = BluetoothSocket(RFCOMM)
        self.uuid = 'cea00e7e-94b8-42fe-9e9f-713b8fb5580b'
        #btle.

    def discover_devices(self):
        devices = bluetooth.discover_devices(duration=30, lookup_names = True)
        return devices

    def serve(self):
        print("serving")
        # self.server_socket.bind(('', PORT_ANY))
        # self.server_socket.listen(1)
        # self.port = self.server_socket.getsockname()[1]

        os.system("hciconfig hci0 piscan")   # make my device discoverable

        results = find_service("HomeAutoService")

        try:
            for result in results:
                print "Trying "+result
                self.server_socket.connect((result['host'], result['port']))
        except BluetoothError:
            print "Could not connect to device"


        #advertise_service(self.server_socket, "HomeAutoService", service_id=self.uuid, service_classes=[self.uuid, SERIAL_PORT_CLASS], profiles=[SERIAL_PORT_PROFILE])

        while True:
            client_sock, client_info = self.server_socket.accept()
            print("Accepted connection from ", client_info)
            #some handshake
            self.connected_sockets.append((client_sock, client_info))

            client_sock.send('{"command": "handshake", "params": ""}')

            data = client_sock.recv(1024)
            if len(data) == 0:
                client_sock.close()

            control.orchestrator.register_module(client_sock, client_info, data)

if __name__ == "__main__":
    bt = BluetoothServer()
    bt.serve()
