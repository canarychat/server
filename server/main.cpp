#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include <functional>

constexpr int BUFFER_SIZE = 1024;
constexpr int EVENT_SIZE = 128;
constexpr int SOCK_SIZE = 128;
constexpr int PORT = 12345;


struct sock_item//listenfd and clientfd
{
    char wbuf[BUFFER_SIZE] = {0};
    char rbuf[BUFFER_SIZE] = {0};
    int wlen = BUFFER_SIZE;
    int rlen = BUFFER_SIZE;
    epoll_event event{};
    sock_item()
    {
        event.data.fd = -1;
    }

};

struct reactor
{
    std::vector<sock_item> socks;
    int epfd;
    epoll_event evs[EVENT_SIZE]{};

    reactor()
    {
        epfd = epoll_create(10);
    }
};

void broad_cast(reactor &r, char *buf, int len)
{
    for (auto &sock: r.socks)
    {
//        if (sock.event.data.fd == -1)
//            continue;
        memcpy(sock.wbuf, buf, len);
        auto ev = sock.event;
        ev.events |= EPOLLOUT;
        epoll_ctl(r.epfd, EPOLL_CTL_MOD, sock.event.data.fd, &ev);
        std::cout << "broad_cast to:"<<sock.event.data.fd << std::endl;
    }
}

int read_line(char *allbuf, int idx, char *linebuf)
{
    auto len = strlen(allbuf);
    for (; idx < len; idx++)
    {
        if (allbuf[idx] == '\r' && allbuf[idx + 1] == '\n')
        {
            linebuf[idx] = '\0';
            return idx + 2;
        } else
            *(linebuf + idx) = allbuf[idx];
    }
}

int hdl_msg(sock_item &si, reactor &r)
{
    char buffer[BUFFER_SIZE];
    memcpy(buffer, si.rbuf, BUFFER_SIZE);
    char linebuf[BUFFER_SIZE];
    memset(linebuf, 0, BUFFER_SIZE);
    read_line(buffer, 0, linebuf);
    std::cout << "firstline: " << linebuf << std::endl;
    if (strstr(linebuf, "\\SYN"))
    {
        memcpy(si.wbuf, "\\SYNACK", 7);
    } else if (strstr(linebuf, "\\MSG"))
    {
        broad_cast(r, buffer + 5, strlen(buffer));
    } else
    {

    }

}


int main()
{
    using namespace std;
    int listenfd;
    reactor r;
    r.socks.reserve(SOCK_SIZE);
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return -1;

    }
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if (bind(listenfd, (sockaddr *) &servaddr, sizeof(servaddr)) == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        std::cerr << "Error binding socket" << std::endl;
        return -2;
    }
    //listen
    if (listen(listenfd, 10) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        return -3;
    }
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenfd;
    epoll_ctl(r.epfd, EPOLL_CTL_ADD, listenfd, &ev);
    while (true)
    {
        int nready = epoll_wait(r.epfd, r.evs, EVENT_SIZE, -1);
        for (int i = 0; i < nready; i++)//i是epoll循环量，不是vec的循环量
        {
            int clientfd = r.evs[i].data.fd;
            if (listenfd == clientfd)
            {
                sockaddr_in client{};
                socklen_t len = sizeof(client);
                int connfd = accept(listenfd, (sockaddr *) &client, &len);
                if (connfd < 0)
                {
                    std::cerr << "Error accepting connection" << std::endl;
                    return -4;
                }
                cout << "new connection--" << (client.sin_addr.s_addr) << ':' << client.sin_port << endl;
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                epoll_ctl(r.epfd, EPOLL_CTL_ADD, connfd, &ev);
                sock_item si{};
                si.event = ev;
                r.socks.push_back(si);
            }
                //clientfd
            else if (r.evs[i].events & EPOLLIN)//IN
            {
                memset(r.socks[i].rbuf, 0, r.socks[i].rlen);
                auto ret = recv(clientfd, r.socks[i].rbuf, r.socks[i].rlen, 0);
                cout << "recving: " << r.socks[i].rbuf << endl;

                if (ret > 0)//client recv properly
                {
                    r.socks[i].rbuf[ret] = '\0';
                    ev = r.socks[i].event;
                    ev.events = EPOLLOUT;
                    ev.data.fd = clientfd;
                    epoll_ctl(r.epfd, EPOLL_CTL_MOD, clientfd, &ev);
                    auto it = find_if(r.socks.begin(), r.socks.end(), [&](sock_item &si) {
                        return si.event.data.fd == clientfd;
                    });
                    hdl_msg(*it, r);
                    //这里有问题，i是epoll循环量，不是vec的循环量
                } else if (ret == 0)//客户端关闭
                {
                    cout << "client closed" << endl;
                    epoll_ctl(r.epfd, EPOLL_CTL_DEL, clientfd, nullptr);
                    if (r.socks.at(i).event.data.fd != -1)
                    {
                        auto it = find_if(r.socks.begin(), r.socks.end(), [&](sock_item &si) {
                            return si.event.data.fd == clientfd;
                        });
                        r.socks.erase(it);
                    }

                    close(clientfd);
                } else
                {
                    cout << "error recv" << endl;
                    epoll_ctl(r.epfd, EPOLL_CTL_DEL, clientfd, nullptr);
                    close(clientfd);
                }

            } else if (r.evs[i].events & EPOLLOUT)//OUT
            {
                send(clientfd, r.socks[i].wbuf, strlen(r.socks[i].wbuf), 0);
                memset(r.socks[i].wbuf, 0, r.socks[i].wlen);
                ev = r.socks[i].event;
                ev.events = EPOLLIN;
                ev.data.fd = clientfd;
                r.socks[i].event = ev;
                epoll_ctl(r.epfd, EPOLL_CTL_MOD, clientfd, &ev);
            }
        }
    }
}

