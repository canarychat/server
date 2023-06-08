# Chat Room

[![license](https://img.shields.io/github/license/Lambert-Rao/chatroom?style=plastic)](LICENSE)[![code size](https://img.shields.io/github/languages/code-size/Lambert-Rao/chatroom?style=plastic)]()[![platform](https://img.shields.io/badge/server-Linux-yellow?style=plastic)](https://en.wikipedia.org/wiki/Linux)[![platform](https://img.shields.io/badge/client-WebBrowser-lightblue?style=plastic)]()

This project is a simple online chat room, built as a learning experience with the POCO library. The chat room consists of both a server, built on Linux, and a client that operates in any web browser.

## Project Structure

The project is organized as follows:

```
.
├── docs: Documentation files including database schemas, state codes, etc.
├── front: HTML files for the client-side user interface.
├── server: Server-side application.
    ├── activeRecordModel: Implementation of the Active Record Design Pattern for database operations.
    ├── data: Manager classes for handling data.
    ├── message: Manager class for handling messages.
    ├── net: Network related files, including the main HTTP server implementation.
    └── support: Supporting files, including error codes and POCO headers.
```

## Background

The motivation behind this project was to learn and gain hands-on experience with the POCO library, a collection of open-source C++ class libraries that simplify and accelerate the development of network-centric, portable applications in C++.

## Installation

Firstly, you need to install POCO:

you can get some help [here](https://cppsecrets.com/users/36681111069711511910546971161041031049711497575664103109971051084699111109/Ubuntu-install-poco-C00-library-Linux.php)
Then you can build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Set MySQL configuration in environment variables. Refer to the [env_var.md](./docs/env_var.md) file in the docs directory for more details.

## Contributing

Issues and Pull Requests are accepted. Feel free to contribute to this project.

## License

[MIT © Richard McRichface.](./LICENSE)
