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
#include <algorithm>

#include "users.h"
#include "message.h"

inline size_t sockaddr_hash(const sockaddr *sa)
{
    auto *sin = (sockaddr_in *) sa;
    std::cout<<sin->sin_port<<std::endl;
    return std::hash<uint32_t>()(sin->sin_addr.s_addr) ^ std::hash<uint16_t>()(sin->sin_port);
}
void read_cb(struct bufferevent *bev, void *ctx)
{
    auto input = bufferevent_get_input(bev);
    if(input == nullptr)
    {
        std::cerr << "bufferevent_get_input failed" << std::endl;
        return;
    }
//    std::bitset<12> version;
//    std::bitset<12> length;
//    std::bitset<8> type;
//    evbuffer_remove(input, &version, 12);
//    evbuffer_remove(input, &length, 12);
//    evbuffer_remove(input, &type, 8);
//
//    if(version==0x010)
//    {
//        char *data = new char[length.to_ulong()];
//        evbuffer_remove(input, data, length.to_ulong());
//        std::ostringstream oss;
//
//        if(type==type::MESSAGE)
//        {
//            broadcast_to_all(data, length.to_ulong());
//        }
//
//    }
    broadcast_to_all(evbuffer_pullup(input, -1));
}
void write_cb(struct bufferevent *bev, void *ctx)
{

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
    bufferevent_free(bev);
}
void accept_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
    auto base = evconnlistener_get_base(listener);
    auto bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, sa);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void accept_error_cb(struct evconnlistener *listener, void *pVoid)
{
    auto base = evconnlistener_get_base(listener);
    std::cerr << "Got an error on the listener." << std::endl;
    event_base_loopexit(base, nullptr);
}