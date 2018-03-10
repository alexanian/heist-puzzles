from flask import Blueprint, render_template

from Config.Cameras import IPS

Views = Blueprint('views', __name__)


# @Views.route('/login')
# def landing():
#     return render_template('login.pug')
#

@Views.route('/feed')
def landing():
    return render_template('feed.pug', cameras=IPS)

#
# @Views.route('/feedback')
# def landing():
#     return render_template('feedback.pug')
