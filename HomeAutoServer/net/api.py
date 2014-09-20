from flask import request
from flask_restful import Resource, reqparse


class System(Resource):
    def get(self):
        return {'status': 'This is some status message'}, 200


class Module(Resource):
    def get(self, module_id):
        return {'status': 'More module messages', 'module_id': module_id}, 200

    def post(self, module_id):
        json_data = request.get_json(force=True)
        operation = json_data['operation']
        return {'status': 'You posted this data', 'data': operation}


class Modules(Resource):
    def get(self):
        return {'status': 'All the modules available'}, 200



