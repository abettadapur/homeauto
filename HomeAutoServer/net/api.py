from flask_restful import Resource, reqparse


class System(Resource):
    def get(self):
        return {'status': 'This is some status message'}, 200


class Module(Resource):
    def get(self):
        return {'status': 'More module messages'}, 200

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('operation', type=str)
        args = parser.parse_args()
        operation = args['operation']
        return {'status': 'You posted this data', 'data': operation}


class Modules(Resource):
    def get(self):
        return {'status': 'All the modules available'}, 200



