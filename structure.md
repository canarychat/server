# How the program structured 

[toc]

## Server 

Epoll is used to listen, recv and send.

the class reactor contains the `vector<sock_item>` which  stores the information of every clientfd's buffer and event. 

## Client 

Tkinter is used for the GUI

classes are used to structure the functional pages.

One Thread in the class chat is listening for messages.

### Status

the array `status` is used to sign the status of  client.

| Type / number  | 0      | 1             |
| :------------- | ------ | ------------- |
| server connect | normal | not connected |
| logged in      | yes    | no            |

## message format 

 [message.h](./server.message/h) lists message types.

| 0~11 bit        | 12~23 bit                            | 24~31 bit    |
| --------------- | ------------------------------------ | ------------ |
| version (0x000) | length (0~4095)(not included header) | message type |

#### LOGIN: userid passwd
#### LOGOUT: userid

#### REGISTER: username passwd

#### MESSAGE: userid message

####  

## TODO 

the next step is to add the account functions.

