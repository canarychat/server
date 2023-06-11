

| 正常 | 0    |
| ---- | ---- |

更新可能不及时, 请以代码为准
[代码](../server/support/state_code.h)

| 中文说明 | 英文描述 | 状态码 |
| ---- | ---- | ---- |
| 正常 | SUCCESS | 0 |
| 注册失败，昵称已存在 | REGISTRATION_NICKNAME_EXISTS | 101 |
| 注册失败，邮件已存在 | REGISTRATION_EMAIL_EXISTS | 102 |
| 注册失败，服务器错误 | USER_SERVER_ERROR | 110 |
| 登录失败，ID不存在 | LOGIN_ID_NOT_EXIST | 121 |
| 登录失败，用户名不存在 | LOGIN_USERNAME_NOT_EXIST | 122 |
| 登录失败，邮件不存在 | LOGIN_EMAIL_NOT_EXIST | 123 |
| 登录失败，密码错误 | LOGIN_PASSWORD_ERROR | 131 |
| 注册失败，客户端错误 | REGISTRATION_CLIENT_ERROR | 151 |
| 登录失败，客户端错误 | LOGIN_CLIENT_ERROR | 152 |
| 验证令牌失败 | VERIFY_TOKEN_FAILED | 181 |
| 创建聊天室失败 | CREATE_CHATROOM_FAILED | 201 |
| 聊天室服务器错误 | CHATROOM_SERVER_ERROR | 210 |
| 聊天室不存在 | CHATROOM_NOT_EXIST | 221 |
| 聊天室用户不存在 | CHATROOM_USER_NOT_EXIST | 222 |
| 聊天室Json无效 | CHATROOM_JSON_INVALID | 231 |
| 聊天室删除非所有者 | CHATROOM_DELETE_NOT_OWNER | 233 |
| 聊天室客户端错误 | CHATROOM_CLIENT_ERROR | 251 |
| 聊天室添加用户失败，用户已在其中 | CHATROOM_ADDUSER_FAILED_USER_ALREADY_IN | 261 |
| 聊天室删除用户失败，用户不在其中 | CHATROOM_DELETEUSER_FAILED_USER_NOT_IN | 262 |
| 聊天室成员服务器错误 | CHATROOM_MEMBER_SERVER_ERROR | 310 |
| 房间成员客户端错误 | ROOM_MEMBER_CLIENT_ERROR | 351 |


