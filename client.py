import socket

from threading import Thread
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "82.157.185.215"
localhost = "127.0.0.1"
sock.connect((localhost, 12345))

def send_msg(sock, msg):
    while True:
        msg = input("Enter your message: ")
        sock.send(msg, "utf8")
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
    print("Connected to server")
    nikename = input("Enter your nikename: ")
    sock.send(nikename)
    Thread(target=send_msg, args=(sock, nikename)).start()
    Thread(target=recv_msg, args=(sock,)).start()