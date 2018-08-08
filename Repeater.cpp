#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif
#ifndef _GLIBCXX_THREAD
#include <thread>
#endif
#ifndef _GLIBCXX_STRING
#include <string>
#endif
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef __CSTDLIB_HEADER
#include <cstdlib>
#endif
#ifndef __CSTRING_HEADER
#include <cstring>
#endif
#ifndef _GLIBCXX_MUTEX
#include <mutex>
#endif
#ifndef _ASM_GENERIC_TYPES_H
#include <sys/types.h>
#endif
#ifndef __ASM_GENERIC_SOCKET_H
#include <sys/socket.h>
#endif
#ifndef _LINUX_IN_H
#include <netinet/in.h>
#endif
#ifndef _ARPA_INET_H
#include <arpa/inet.h>
#endif
#include <unistd.h>

unsigned char *Dosomething(unsigned char *c)
{
    static unsigned char result[256] = {0};
    for (int i = 0; i < 255; ++i)
    {
        result[i + 1] = c[i];
    }
    return result;
}
void SendMessage(unsigned char *Message, int sendToPort)
{
    int client_sockfd = 0;
    int client_addr_len = 0;
    struct sockaddr_in client_addr;
    int n = 0;
    int connectionResult = 0;
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_port = htons(sendToPort);
    client_addr_len = sizeof(client_addr);
    connectionResult = connect(client_sockfd, (struct sockaddr *)&client_addr, client_addr_len);
    n = write(client_sockfd, Message, 256);
    close(client_sockfd);
}
int main(int argc, char const *argv[])
{
    int listenPort = 0;
    int sendToPort = 0;
    std::cout << "Reveive From (Port): ";
    std::cin >> listenPort;
    getchar();
    std::cout << "Send to (Port): ";
    std::cin >> sendToPort;
    getchar();

    int host_sockfd = 0;
    int client_sockfd = 0;
    int host_addr_len = 0;
    int client_addr_len = 0;
    struct sockaddr_in host_addr;
    struct sockaddr_in client_addr;
    host_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    host_addr.sin_family = AF_INET;
    host_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    host_addr.sin_port = htons(listenPort);
    host_addr_len = sizeof(host_addr);
    bind(host_sockfd, (struct sockaddr *)&host_addr, host_addr_len);
    listen(host_sockfd, 2);
    client_addr_len = sizeof(client_addr);
    while (true)
    {
        client_sockfd = accept(host_sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
        std::cout << "Reveived Message. " << std::endl;
        unsigned char buffer[256] = {0};
        int n = read(client_sockfd, buffer, 256);
        std::cout << "Doing Something. " << std::endl;
        unsigned char *newbuffer = nullptr;
        newbuffer = Dosomething(buffer);
        SendMessage(newbuffer, sendToPort);
        std::cout << "New Message sent. " << std::endl;
    }
    return 0;
}
