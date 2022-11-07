import socket
from threading import Thread

host = '0.0.0.0'
port = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))
clients = []


def broadcast(msg, nikename=""):
    for sock in clients:
        sock.send(bytes(nikename, "utf8") + msg)


def handle_client(conn, addr):
    print("Connected to: ", addr)
    nickname = conn.recv(1024).decode("utf8")
    clients.append(conn)
    welcome = '%s has joined!' % nickname
    broadcast(bytes(welcome, "utf8"))
    while True:
        msg = conn.recv(1024)
        if msg != bytes("{quit}", "utf8"):
            broadcast(msg, nickname + ": ")
        else:
            conn.send(bytes("{quit}", "utf8"))
            conn.close()
            del clients[clients.index(conn)]
            print("Disconnected from: ", addr, nickname)
            broadcast(bytes("%s has left the chat." % nickname, "utf8"))
            break


if __name__ == '__main__':
    print("Server is running...")
    sock.listen(5)
    while True:
        print("Waiting for connection...")
        client, address = sock.accept()
        Thread(target=handle_client, args=(client, address)).start()
