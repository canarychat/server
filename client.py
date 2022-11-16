import socket
import tkinter

from threading import Thread

from distlib.compat import raw_input

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "82.157.185.215"
localhost = "127.0.0.1"
sock.connect((host, 12345))


if __name__== '__main__':


