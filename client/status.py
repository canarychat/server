def judge_status(status) -> str:
    s = "|"
    if status[0] == 1:
        s += "未连接服务器|"
    if status[1] == 1:
        s += "未登录|"

    return s