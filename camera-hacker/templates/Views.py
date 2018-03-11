from flask import Blueprint, render_template, redirect, request

from Config.Cameras import IPS
from Config.Security import TOKEN

Views = Blueprint('views', __name__)


@Views.route('/login')
def login():
    return render_template('login.pug')


@Views.route('/feed')
def landing():
    token = request.args.get('token')
    if token and token == TOKEN:
        return render_template('feed.pug', cameras=IPS)
    else:
        return redirect('/login')


@Views.route('/feedback')
def feedback():
    return render_template('feedback.pug')
