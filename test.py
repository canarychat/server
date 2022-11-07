import socket
from threading import Thread
host = '0.0.0.0'
port = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))

sock.listen(5)
client, address = sock.accept()
print("Connected to: ", address)
nikename = client.recv(1024).decode("utf8")
client.send(bytes("Welcome %s!" % nikename, "utf8"))