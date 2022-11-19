import os
import re
import sys
import time
import tkinter as tk
import tkinter.messagebox as me
import socket

from status import judge_status
from handle_config import *

from threading import Thread

resolution = []


# 网络部分
def send_msg(msg):
    try:
        sock.send(bytes(msg, "ascii"))
    except:
        return False
def recv_msg():
    try:
        return sock.recv(1024).decode('ascii')
    except:
        return False
def test(h, p):
    host.set(h)
    port.set(p)
    try:
        sock.send(bytes('\\SYN', "ascii"))
    except:
        return False
def connect():
    try:
        sock.close()
    except:
        pass
    restart_program()
def disconnect():
    sock.close()
    status[0] = 1
    host.set("未连接服务器")
    refresh_menu()
# 界面部分

class text_interface:
    def __init__(self):
        self.frame = tk.Frame(root, width=400, height=300, bg="light blue")
        self.welcome = tk.Label(self.frame, text="欢迎使用聊天室", bg="light blue", fg="black",
                                font=('Times', 20, 'bold italic'), borderwidth=10, padx=10, pady=10, relief="groove")
        self.warning = tk.Label(self.frame, text="目前仅支持ASCII", bg="yellow", fg="black",
                                font=('Times', 10, 'bold italic'), borderwidth=10, padx=10, pady=10, relief="groove")
        self.text = tk.Text(self.frame, width=50, height=10, font=("黑体", 12), state='normal')
        self.board = tk.Text(self.frame, width=50, height=5, undo=True, font=("黑体", 12), state='normal')
        self.send = tk.Button(self.frame, text="发送", font=("黑体", 12), command=self.button_send_msg)
        self.frame.grid(row=1, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        self.welcome.grid(row=0, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        self.warning.grid(row=1, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        self.text.grid(row=2, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        self.board.grid(row=3, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        self.send.grid(row=4, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
    def button_send_msg(self):
        if (status[0] == 1):
            me.showerror(title="错误", message="未连接服务器")
        msg = self.board.get("1.0", "end")
        self.board.delete("1.0", "end")
        if (msg == ""):
            return
        if (len(msg) > 1000):
            me.showerror("错误", "消息过长")
            return
        if ('\\' in msg):
            me.showerror("错误", "消息中不能包含\\")
            return
        msg = '\\MSG ' + name.get() + ":" + msg
        send_msg(msg)

    def forget(self):
        self.frame.grid_forget()
class init_interface:
    def __init__(self):
        root.title("聊天室")
        root.geometry("800x900+200+300")
        root.minsize(400, 300)
        resolution.append(root.winfo_screenwidth())
        resolution.append(root.winfo_screenheight())
        root.maxsize(resolution[0], resolution[1])
        root.resizable(width=True, height=True)
        root.configure(background='light blue')
        self.welcome = tk.Label(root, text='欢迎使用聊天室', bg='light blue', font=('微软雅黑', 20))
        self.welcome.grid()

    def forget(self):
        self.welcome.grid_forget()
class chat:
    def __init__(self):
        forget()
        T.__init__()

    def refresh_text(self):
        Thread(target=self.listen).start()

    def listen(self):
        while (True):
            msg = sock.recv(1024).decode('ascii')
            print(msg)
            T.text.insert(1.0, msg)
            time.sleep(0.1)
class switch:
    def __init__(self):
        I.forget()
        T.forget()
        self.server_frame = tk.Frame(root, width=400, height=300, bg="light blue")
        self.server_frame.grid(row=0, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        text01 = tk.Label(self.server_frame, text="服务器地址", bg="green", font=("黑体", 12))
        text01.grid(row=2, column=1, sticky=tk.N + tk.S + tk.E + tk.W)
        text02 = tk.Label(self.server_frame, text="端口号", bg="green", font=("黑体", 12))
        text02.grid(row=3, column=1, sticky=tk.N + tk.S + tk.E + tk.W)
        self.host = tk.Text(self.server_frame, width=20, height=1, bg='white', font=("黑体", 12))
        self.port = tk.Text(self.server_frame, width=20, height=1, bg='white', font=("黑体", 12))
        self.host.grid(row=2, column=2, sticky=tk.N + tk.S + tk.E + tk.W)
        self.port.grid(row=3, column=2, sticky=tk.N + tk.S + tk.E + tk.W)
        self.host.insert("1.0", get_host())
        self.port.insert("1.0", str(get_port()))
        accept = tk.Button(self.server_frame, text="确定", font=("黑体", 12), command=self.get)
        accept.grid(row=5, column=3, sticky=tk.N + tk.S + tk.E + tk.W)

    def get(self):
        h = self.host.get(1.0, "1.end")
        self.host.delete(1.0, "1.end")
        p = self.port.get(1.0, "1.end")
        self.port.delete(1.0, "1.end")
        if re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$", h) and re.match(r"^\d{1,5}$", p):
            chage_server(h, p)
            me.showinfo("提示", "服务器地址修改成功,将重启程序")
            restart_program()
        else:
            me.showerror("错误", "输入格式错误", parent=root)

    def forget(self):
        self.server_frame.grid_forget()
def forget():
    T.forget()
    I.forget()
    S.forget()
# 菜单部分
def refresh_menu():
    # menu
    main_menu = tk.Menu(root)

    main_menu.add_command(label="聊天", font=("黑体", 12), command=chat)

    accountmenu = tk.Menu(main_menu, tearoff=0)
    accountmenu.add_command(label="注册", font=("黑体", 12), command=signin)
    accountmenu.add_command(label="登录", font=("黑体", 12), command=login)
    accountmenu.add_command(label="登出", font=("黑体", 12), command=logout)
    accountmenu.add_separator()
    accountmenu.add_command(label="修改昵称", font=("黑体", 12), command=change_name)
    accountmenu.add_command(label="修改密码", font=("黑体", 12), command=change_password)
    main_menu.add_cascade(label=name.get(), menu=accountmenu, font=("黑体", 12))

    servermenu = tk.Menu(main_menu, tearoff=0)
    servermenu.add_command(label=host.get(), font=("黑体", 12))
    servermenu.add_command(label="连接", font=("黑体", 12), command=connect)
    servermenu.add_command(label="断开", font=("黑体", 12), command=disconnect)
    servermenu.add_command(label="切换", font=("黑体", 12), command=S.__init__)
    main_menu.add_cascade(label="服务器", menu=servermenu, font=("黑体", 12))
    s = judge_status(status)
    if (s != "|"):
        main_menu.add_command(label=s, font=("黑体", 12))
    root.configure(menu=main_menu)

# 按钮部分
def login():
    forget()
    if (status[0] == 1):
        me.showerror("错误", "请先连接服务器", parent=root)
    me.showwarning("警告", "该功能开发中", parent=root)
def signin():
    forget()
    me.showwarning("警告", "该功能开发中", parent=root)
def logout():
    forget()
    me.showwarning("警告", "该功能开发中", parent=root)
class change_name():
    def __init__(self):
        forget()
        self.frame = tk.Frame(root, width=400, height=300, bg="light blue")
        self.frame.grid(row=0, column=0, sticky=tk.N + tk.S + tk.E + tk.W)
        text01 = tk.Label(self.frame, text="新昵称", bg="green", font=("黑体", 12))
        text01.grid(row=2, column=1, sticky=tk.N + tk.S + tk.E + tk.W)
        self.newname = tk.Text(self.frame, width=20, height=1, bg='white', font=("黑体", 12))
        self.newname.grid(row=2, column=2, sticky=tk.N + tk.S + tk.E + tk.W)
        accept = tk.Button(self.frame, text="确定", font=("黑体", 12), command=self.get)
        accept.grid(row=5, column=3, sticky=tk.N + tk.S + tk.E + tk.W)

    def get(self):
        h = self.newname.get(1.0, "1.end")
        self.newname.delete(1.0, "1.end")
        if ('\\' in h):
            me.showerror("错误", "昵称中不能使用反斜杠\\")
            change_name()
        if (len(h) > 20):
            me.showerror("错误", "昵称过长")
            change_name()
        if (len(h) == 0):
            me.showerror("错误", "昵称不能为空")
            change_name()
        else:
            name.set(h)
            set_id("-1")
            set_name(h)
            self.frame.grid_forget()
            refresh_menu()
def change_password():
    forget()
    me.showwarning("警告", "该功能开发中", parent=root)
# 其他功能
def restart_program():
    python = sys.executable
    os.execl(python, python, *sys.argv)

# 界面
root = tk.Tk()
I = init_interface()
T = text_interface()

S = switch()
C = chat()


if __name__ == '__main__':
    status = [1, 1]
    name = tk.StringVar()
    name.set(get_name())
    host = tk.StringVar()
    port = tk.IntVar()
    host.set(get_host())
    port.set(get_port())
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host.get(), port.get()))
        status[0] = 0
    except:
        pass
    I.__init__()
    refresh_menu()
    C.refresh_text()
    forget()
    root.mainloop()

