import threading
from flask import Flask
from flask_restful import Api
import time
from net import api as restapi
from bt.btcontroller import BtController
from gevent.pywsgi import WSGIServer
import gevent
from gevent import monkey

app = Flask(__name__)
Bt = BtController()
api = Api(app)

api.add_resource(restapi.System, '/system')
api.add_resource(restapi.Modules, '/modules')
api.add_resource(restapi.Module, '/module/<string:module_id>')


if __name__ == '__main__':
    http_server = WSGIServer(
        ('0.0.0.0', 5000),
        app
    )

    http_server.serve_forever()


