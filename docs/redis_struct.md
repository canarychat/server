
消息
```redis
chatroom:1008:message [
    '{"sender_id": 1, "timestamp": 1642422240, "message_type": "text/plain", "content": "Hello World"}',
    '{"sender_id": 2, "timestamp": 1642422250, "message_type": "text/plain", "content": "Hello too"}'
]
```


```redis
SET user:<user_id>:online true EX 60
EXISTS user:<user_id>:online
```
