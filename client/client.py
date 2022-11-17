import socket
import tkinter as tk

from threading import Thread

from distlib.compat import raw_input

HOST = "82.157.185.215"


if __name__== '__main__':
    root = tk.Tk()  # 创建窗口对象的背景色
    # 创建两个列表
    li = ['C', 'python', 'php', 'html', 'SQL', 'java']
    movie = ['CSS', 'jQuery', 'Bootstrap']
    listb = tk.Listbox(root)  # 创建两个列表组件
    listb2 = tk.Listbox(root)
    for item in li:  # 第一个小部件插入数据
        listb.insert(0, item)

    for item in movie:  # 第二个小部件插入数据
        listb2.insert(0, item)

    listb.pack()  # 将小部件放置到主窗口中
    listb2.pack()
    root.mainloop()
