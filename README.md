# Chat room

[![license](https://img.shields.io/github/license/Lambert-Rao/chatroom?style=plastic)](LICENSE)[![code size](https://img.shields.io/github/languages/code-size/Lambert-Rao/chatroom?style=plastic)]()[![platform](https://img.shields.io/badge/server-Linux-yellow?style=plastic)](https://en.wikipedia.org/wiki/Linux)[![platform](https://img.shields.io/badge/client-Linux/Windows-lightblue?style=plastic)]()

A simple client-server chat room project.


## Background

I'm learning socket program, so I make this as a practice.

## Install

```shell
#get all resources 
wget https://github.com/Lambert-Rao/chatroom.git

#sometimes you need to install the tk
sudo pacman -S tk
```

## Usage

```shell
#for client
python3 client.py
```


```shell
#for server 
#download the server directory 
mkdir cmake & cd cmake & cmake .. & make
./server ${your_port}
```

## Contributing

issues and PRs accepted.

Reading the [structure](structure.md) file to contribute.

## License

[MIT © Richard McRichface.](./LICENSE)