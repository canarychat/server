//
// Created by lambert on 22-12-11.
//

#pragma once
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <string>
#include <unordered_map>
#include <vector>

struct User
{
    bufferevent *bev;
    std::string username;
    uint32_t id;
};
//std::unordered_map<uint32_t ,User> user_map;
std::vector<User> user_vec;
void broadcast_to_all(evbuffer *input)
{
    for(auto &user : user_vec)
    {
        evbuffer_add_buffer(user.bev->output_buffer, input);
    }
}