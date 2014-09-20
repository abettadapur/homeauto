from flask import Flask
from flask_restful import Api
from net import api as restapi

app = Flask(__name__)
api = Api(app)

api.add_resource(restapi.System, '/system')
api.add_resource(restapi.Modules, '/modules')
api.add_resource(restapi.Module, '/module/<string:module_id>')


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, use_reloader=False)
