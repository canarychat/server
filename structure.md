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

| 0~11 bit        | 12~23 bit                          | 24~31 bit    |
| --------------- | ---------------------------------- | ------------ |
| version (0x000) | length (0~4095)(not included head) | message type |

#### LOGIN: userid passwd
#### LOGOUT: userid

#### REGISTER: username passwd

#### MESSAGE: userid message

 ```mermaid
   
flowchart
subgraph server
t{task}
subgraph account
p1[login]
p2[register]
db1[(Redis)]
end
p1 <--> db1
p2 <--> db1
t --> p3[message]
t --> p2
t --> p1
db1 <--> db2
db2[(mySql)]
end

subgraph client
s1([start]) --> connect

connect --> login
connect --> signup
login --> chat
login -.LOGIN.- p1
connect --> e
signup --> chat
signup -.REGISTER.- p2
chat -.MESSAGE.- p3
chat --> e([end])
end

 ```

## TODO 

the next step is to add the account functions.

