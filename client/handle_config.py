import json


def load_config():
    file_object = open("config.json", "r")
    config = json.loads(file_object.read())
    file_object.close()
    return config
def save_config(config):
    file_object = open("config.json", "w")
    file_object.write(json.dumps(config))
    file_object.close()

def get_name():
    config = load_config()
    if(config["usr"]["id"] == ""):
        return "未登录"
    else:
        return config["usr"]["name"]
def set_name(name):
    config = load_config()
    config["usr"]["name"] = name
    save_config(config)
def get_id():
    config = load_config()
    return config["usr"]["id"]
def set_id(id):
    config = load_config()
    config["usr"]["id"] = id
    save_config(config)
def get_host()->str:
    config = load_config()
    return config["server"]["host"]
def get_port()->int:
    config = load_config()
    return int(config["server"]["port"])
def chage_server(host,port):
    config = load_config()
    config["server"]["host"] = host
    config["server"]["port"] = port
    save_config(config)
