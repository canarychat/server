//
// Created by lambert on 22-12-11.
//

#pragma once

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <iostream>
#include <bitset>
#include <sstream>
#include <cstring>
#include <array>

#include "users.h"
#include "message.h"

inline size_t sockaddr_hash(const sockaddr *sa)
{
    std::string sa_data;
    sa_data.resize(14);
    for(int i = 0; i < 14; i++)
    {
        sa_data[i] = sa->sa_data[i];
    }
   return std::hash<std::string>{}(sa_data);
}
void read_cb(struct bufferevent *bev, void *ctx)
{
    auto input = bufferevent_get_input(bev);
    if(input == nullptr)
    {
        std::cerr << "bufferevent_get_input failed" << std::endl;
        return;
    }
    std::bitset<12> version;
    std::bitset<12> length;
    std::bitset<8> type;
    evbuffer_remove(input, &version, 12);
    evbuffer_remove(input, &length, 12);
    evbuffer_remove(input, &type, 8);

    if(version==0x010)
    {
        char *data = new char[length.to_ulong()];
        evbuffer_remove(input, data, length.to_ulong());
        std::ostringstream oss;

        if(type==type::MESSAGE)
        {
            broadcast_to_all(data, length.to_ulong());
        }

    }
}
void write_cb(struct bufferevent *bev, void *ctx)
{
    auto h = sockaddr_hash(static_cast<sockaddr *>(ctx));
    std::cout<<static_cast<sockaddr *>(ctx)<<std::endl;
    std::cout<<h<<std::endl;
}
void event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_EOF)
    {
        std::cout << "Connection closed." << std::endl;
    }
    else if (events & BEV_EVENT_ERROR)
    {
        std::cerr << "Got an error on the connection: " << strerror(errno) << std::endl;
    }
    auto h = sockaddr_hash(static_cast<sockaddr *>(ctx));
    user_map.erase(h);
    bufferevent_free(bev);
}
void accept_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
    auto base = evconnlistener_get_base(listener);
    auto bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    //对sockaddr中的sa_data做hash
    size_t h = sockaddr_hash(sa);
    std::cout<<sockaddr_hash(sa)<<std::endl;
    user_map[h] = User{bev};
    std::cout<<sa<<std::endl;
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, sa);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void accept_error_cb(struct evconnlistener *listener, void *pVoid)
{
    auto base = evconnlistener_get_base(listener);
    std::cerr << "Got an error on the listener." << std::endl;
    event_base_loopexit(base, nullptr);
}