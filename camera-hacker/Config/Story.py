from Config.Security import USER, PASSWD
from Config.SharedState import VARIABLES

RESPONSES = {
    "ls": "instructions.txt       server.php       Config",
    "ls-config": "options.json",
    "rm": "Command disabled",
    "php": "Current running instance",
    "fail": "permission denied",
    "success": "File created",
    "where": "File not found",
    "else": "Command not found. Try running sudo apt-get install to get it.",
    "sudo": "Feature currently disabled",
    "cat-config": "{{frozen: {}}}".format({0: 'false', 1: 'true'}[VARIABLES['camera_frozen']]),
    "cat": "Reg how many times do I need to tell you these things! " +
    "Your username is {u} and your password is {p}. ".format(u=USER, p=PASSWD) +
    "If you want to pause the cameras so your boss can't see whatever it is you and your boyfriend" +
    " get up to when he isn't around, you need to go to the terminal window and type: " +
    "`rm Config/options.json;cat {frozen: true} > Config/options.json`." +
    "To put it back afterwards, just type `rm Config/options.json;cat {frozen: false} > "
    "Config/options.json`. Please for the love of god stop emailing me. I don't want to hear about " +
    "what you're getting up to!"
}
