//
// Created by lambert on 22-12-11.
//
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <hiredis/hiredis.h>

#include <iostream>

#include "callbacks.h"

uint16_t PORT = 8888;

int main(int argc, char **argv) {

    using namespace std;
    //初始参数判断
    if (argc == 2)
    {
        PORT = strtol(argv[1], nullptr, 10);
        if(PORT == 0 || PORT > 65535)
        {
            cout << "invalid port" << endl;
            return -1;
        }
    } else if (argc >= 3 or argc == 1)
    {
        cout << "default port is " << PORT << "\nif you want to "
                                              "use your own port, using like this:\n"
                                              "./server [PORT]" << endl;
    }


    auto base = event_base_new();
    if (!base)
    {
        cerr << "event_base_new failed" << endl;
        return -1;
    }

    auto sin = sockaddr_in{0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    auto listener = evconnlistener_new_bind(base, accept_cb, base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                            (struct sockaddr *) &sin, sizeof(sin));

    evconnlistener_set_error_cb(listener, accept_error_cb);

    event_base_loop(base, 0);
}