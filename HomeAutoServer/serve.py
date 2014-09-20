from flask import Flask
from flask_restful import Api
from net import api as restapi

app = Flask(__name__)
api = Api(app)

api.add_resource(rest)
