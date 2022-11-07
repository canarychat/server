import socket

from threading import Thread

from distlib.compat import raw_input

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "82.157.185.215"
localhost = "127.0.0.1"
sock.connect((host, 12345))

def send_msg(sock, msg):
    while True:
        msg = raw_input()
        sock.send(bytes(msg, "utf8"))
        if msg == "{quit}":
            sock.close()
            break
def recv_msg(sock):
    while True:
        try:
            data = sock.recv(1024)
            print(data.decode("utf8"))
        except OSError:
            break
if __name__== '__main__':
    print("Connected to server,type'{quit}' to exit")
    nikename = raw_input("Enter your nikename: ")
    sock.send(bytes(nikename, "utf8"))
    Thread(target=send_msg, args=(sock, nikename)).start()
    Thread(target=recv_msg, args=(sock,)).start()