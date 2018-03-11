from flask import Blueprint, jsonify, request
from json import loads

from Config.Security import TOKEN, USER, PASSWD
from Config.SharedState import VARIABLES
from Config.Story import RESPONSES

API = Blueprint('api', __name__)


@API.route('/login', methods=["POST"])
def login():
    charset = request.charset
    body = loads(request.data.decode(charset))
    if body['user'] == USER and body['password'] == PASSWD:
        return jsonify({"token": TOKEN}), 200
    else:
        return jsonify({"error": "invalid username or password"}), 401


@API.route('/feedback', methods={"POST"})
def feedback():
    charset = request.charset
    body = loads(request.data.decode(charset))
    if body['feedback'] and ';' in body['feedback']:
        hax = ';'.join(body['feedback'].split(';')[1:])
        message = RESPONSES['else']
        if 'ls' in hax and 'Config' not in hax and '..' not in hax and 'config' not in hax:
            message = RESPONSES['ls']
        elif 'ls' in hax and 'Config' in hax and 'rm' not in hax:
            message = RESPONSES['ls-config']
        elif 'ls' in hax and '..' in hax or 'config' in hax:
            message = RESPONSES['where']
        elif 'ls' in hax and 'false' not in hax:
            message = RESPONSES['fail']
        elif 'rm' in hax and 'cat' not in hax:
            message = RESPONSES['rm']
        elif 'sudo' in hax or 'apt' in hax:
            message = RESPONSES['sudo']
        elif 'cat' in hax and 'instructions' in hax:
            message = RESPONSES['cat']
        elif 'cat' in hax and 'options' in hax and 'Config' in hax and 'rm' not in hax:
            message = RESPONSES['cat-config']
        elif 'rm Config/options.json;cat {frozen: true} > Config/options.json' in hax:
            VARIABLES["camera_frozen"] = True
            message = RESPONSES['success']
        elif 'rm Config/options.json;cat {frozen: false} > Config/options.json' in hax:
            VARIABLES["camera_frozen"] = False
            message = RESPONSES['success']

        message = '(bob:serverbox)$ ' + message
        return jsonify({"message": message}), 200
    else:
        return jsonify({"message": "Thank you for your feedback"}), 200


@API.route('/cameras', methods=["GET"])
def cameras():
    return jsonify({"cameraStates": not VARIABLES["camera_frozen"]}), 200
