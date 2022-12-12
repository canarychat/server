//
// Created by lambert on 22-12-11.
//

#pragma once
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <string>
#include <unordered_map>

struct User
{
    bufferevent *bev;
    std::string username;
    uint32_t id;
};
std::unordered_map<size_t ,User> user_map;

void broadcast_to_all(const char *data, size_t len)
{
    for(auto &user : user_map)
    {
        bufferevent_write(user.second.bev, data, len);
    }
}