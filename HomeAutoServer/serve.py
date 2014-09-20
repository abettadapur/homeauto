from flask import Flask
from flask_restful import Api
from net import api as restapi
from bt.btcontroller import BluetoothServer
from gevent.pywsgi import WSGIServer
import gevent

app = Flask(__name__)
BtController = BluetoothServer()
api = Api(app)

api.add_resource(restapi.System, '/system')
api.add_resource(restapi.Modules, '/modules')
api.add_resource(restapi.Module, '/module/<string:module_id>')


if __name__ == '__main__':
    http_server = WSGIServer(
        ('0.0.0.0', 5000),
        app
    )


    btgl = gevent.spawn(BtController.serve())
    httpgl = gevent.spawn(http_server.serve_forever())
    btgl.start()
    httpgl.start()


