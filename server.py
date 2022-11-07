import socket
from threading import Thread

host = '0.0.0.0'
port = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))
client = {}
addresses = {}


def broadcast(msg, nikename=""):
    for sock in client:
        sock.send(bytes(nikename, "utf8") + msg)


def handle_client(conn, addr):
    addstr = str(address)
    print("Connected to: ", addr)
    nikename = conn.recv(1024).decode("utf8")
    # client[addstr] = nikename
    # addresses[client] = addr
    welcome = 'Welcome %s!' % nikename
    broadcast(bytes(welcome, "utf8"))
    while True:
        msg = conn.recv(1024)
        if msg != bytes("{quit}", "utf8"):
            broadcast(msg, nikename + ": ")
        else:
            conn.send(bytes("{quit}", "utf8"))
            conn.close()
            del client[addstr]
            broadcast(bytes("%s has left the chat." % nikename, "utf8"))
            break


if __name__ == '__main__':
    print("Server is running...")
    sock.listen(5)
    while True:
        print("Waiting for connection...")
        client, address = sock.accept()
        Thread(target=handle_client, args=(client, address)).start()
