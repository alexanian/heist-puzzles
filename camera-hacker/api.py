from flask import Blueprint, jsonify, request, send_file
from json import loads
from glob import glob
from os import path

from Config.Security import TOKEN, USER, PASSWD
from Config.SharedState import VARIABLES
from Config.Story import RESPONSES
from get_images import convert_ip_to_safe

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
        elif ('cat' in hax or 'vi' in hax or 'nano' in hax) and 'instructions.txt' in hax:
            message = RESPONSES['cat']
        elif ('cat' in hax or 'vi' in hax or 'nano' in hax) in hax and 'options' in hax \
                and 'Config' in hax and 'rm' not in hax:
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


@API.route('/feeds/<feed>')
def get_last_image(feed):
    file_base = convert_ip_to_safe(feed) + '*'
    file_path = path.join('.', 'images', file_base)
    f = sorted([f for f in glob(file_path)])[-1]
    return send_file(f, mimetype='image/jpg')


@API.route('/cameras', methods=["GET"])
def cameras():
    return jsonify({"cameraStates": not VARIABLES["camera_frozen"]}), 200
