from flask import Blueprint, jsonify, request
from json import loads

from Config.Security import TOKEN
from Config.SharedState import VARIABLES

API = Blueprint('api', __name__)


@API.route('/login', methods=["POST"])
def login():
    charset = request.charset
    body = loads(request.data.decode(charset))
    if body['user'] == 'bobby@acmesecurity.com' and body['password'] == 'something1clever':
        return jsonify({"token": TOKEN}), 200
    else:
        return jsonify({"error": "invalid username or password"}), 401


@API.route('/feedback', methods={"POST"})
def feedback():
    charset = request.charset
    body = loads(request.data.decode(charset))
    if body['feedback'] and ';' in body['feedback']:
        VARIABLES["camera_frozen"] = not VARIABLES["camera_frozen"]
        return jsonify({"message": "l33t h4xor"}), 200
    else:
        return jsonify({"message": "Thank you for your feedback"}), 200


@API.route('/cameras', methods=["GET"])
def cameras():
    return jsonify({"cameraStates": not VARIABLES["camera_frozen"]}), 200
