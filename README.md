# Chat room

[![license](https://img.shields.io/github/license/Lambert-Rao/chatroom?style=plastic)](LICENSE)[![code size](https://img.shields.io/github/languages/code-size/Lambert-Rao/chatroom?style=plastic)]()[![platform](https://img.shields.io/badge/platform-Linux-yellow?style=plastic)](https://en.wikipedia.org/wiki/Linux)

A simple client-server chat room project.

> warning! may not work on windows platform

## Background

I'm learning socket program, so I make this as a practice.

## Install

```shell
#get all resources 
wget https://github.com/Lambert-Rao/chatroom.git
#or install from release 

#sometimes you need to install the tk
sudo pacman -S tk
```

## Usage

```shell
#for client
python3 client.py
#or use the packaged executable file
```


```shell
#for server 
#download the server directory 
cd cmake & cmake .. & make
./server ${your_port}
```

## Contributing

issues and PRs accepted.

## License

[MIT © Richard McRichface.](./LICENSE)