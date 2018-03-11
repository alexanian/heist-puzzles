from flask import Flask

from api import API
from templates.Views import Views
from Config.Server import HOST, PORT

app = Flask(__name__)

app.jinja_env.add_extension('pypugjs.ext.jinja.PyPugJSExtension')

host = 'localhost'
port = 8080

app.register_blueprint(Views, url_prefix='')
app.register_blueprint(API, url_prefix='/api')

# you're going to want to change BOTH `HOST` and Debug for production
app.run(HOST, port=PORT, debug=True)

