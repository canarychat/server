# How the program structured 

[toc]

## Server 

I use poco lib.

## Client 

clinet not developed

## http/ws api

> 待v1开发完成确定后导出

![image-20230526134622077](https://raw.githubusercontent.com/Limpol-Rao/image_host/main/img/202305261346289.png)

## Websocket API
```json
{
    "type": "messageType", //消息类型，比如"text/plain", "Authorization/jwt", "HistoryRequest", "SystemControl"等
    "timestamp": 1618881585, //消息发送的Unix时间戳
    "sender": { 
        "id": 1008, //发送者ID
        "name": "L" //发送者昵称
    },
    "content": "messageContent", //消息内容，具体的结构取决于消息类型
}
```

- 文本消息：

```json
{
    "type": "text/plain",
    "timestamp": 1618881585,
    "sender": {
        "id": 1008,
        "name": "L"
    },
    "content": "Hello, world!"
}
```

### Authorization/jwt
- Client Send：

```json
{
    "type": "Authorization/jwt",
    "timestamp": 1618881585,
    "sender": {
        "id": 1008,
        "name": "L"
    },
    "content": "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```
- Server Send：

```json
{
    "type": "Authorization/jwt",
    "timestamp": 1618881585,
    "sender": {
        "id": 100,
        "name": "server"
    },
    "content": {
        "code": 0, 
        "msg": "JWT verified!" 
    }
}
```

```json
{
  "type": "Authorization/jwt",
  "timestamp": 1618881585,
  "sender": {
    "id": 100,
    "name": "server"
  },
  "content": {
    "code": not 0,
    "msg": "JWT failed!"
  }
}
```

- 历史消息请求：

```json
{
    "type": "HistoryRequest",
    "timestamp": 1618881585,
    "sender": {
        "id": 1008,
        "name": "L"
    },
    "content": {
        "start": 1618800000, //请求历史消息的开始时间戳
        "end": 1618881585 //请求历史消息的结束时间戳
    }
}
```

- 系统控制消息：

```json
{
    "type": "SystemControl",
    "timestamp": 1618881585,
    "sender": {
        "id": 1008,
        "name": "L"
    },
    "content": {
        "command": "kick", //命令类型，如"kick"（踢人）、"ban"（禁言）等
        "target": 1009 //目标用户的ID
    }
}
```