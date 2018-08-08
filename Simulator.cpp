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

char *Hex2String(unsigned char c)
{
    static char result[3] = {0};
    if ((c >> 4) >= 10)
    {
        result[0] = (c >> 4) - 10 + 'a';
    }
    else
    {
        result[0] = (c >> 4) + '0';
    }
    if ((c & 0x0f) >= 10)
    {
        result[1] = (c & 0x0f) - 10 + 'a';
    }
    else
    {
        result[1] = (c & 0x0f) + '0';
    }
    return result;
}
unsigned char Char2Hex(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return 0;
}
unsigned char TwoChars2Hex(char *str)
{
    unsigned char result = 0;
    result = Char2Hex(str[0]) << 4 | Char2Hex(str[1]);
    return result;
}
void ListenPort(int listenPort)
{
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
        unsigned char buffer[256] = {0};
        int n = read(client_sockfd, buffer, 256);
        std::cout << "Received Message. " << std::endl;
        close(client_sockfd);

        std::cout << "Original Content: " << std::endl;
        std::cout << "   ";
        for (int i = 0; i < 16; ++i)
        {
            printf("|%02d", i);
        }
        std::cout << std::endl;
        for (int i = 0; i < 51; ++i)
        {
            std::cout << "-";
        }
        std::cout << std::endl;
        for (int i = 0; i < 16; ++i)
        {
            printf("%02d", i);
            std::cout << " ";
            for (int j = 0; j < 16; ++j)
            {
                std::cout << "|" << Hex2String(buffer[i * 16 + j]);
            }
            std::cout << std::endl;
        }
    }
}
void SendHexMessage(unsigned char *hexMessage, int sendToPort)
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
    n = write(client_sockfd, hexMessage, 256);
    close(client_sockfd);
}
int main(int argc, char const *argv[])
{
    int listenPort = 0;
    int sendToPort = 0;
    std::cout << "Receive From (Port): ";
    std::cin >> listenPort;
    getchar();
    std::cout << "Send To (Port): ";
    std::cin >> sendToPort;
    getchar();

    auto listenTask = std::thread(ListenPort, listenPort);

    while (true)
    {
        char inputChars[512] = {0};
        int p = 0;
        char c = 0;
        c = getchar();
        while (c != '\n' && p < 512)
        {
            if (c != ' ')
            {
                inputChars[p] = c;
                ++p;
            }
            c = getchar();
        }
        unsigned char hexMessage[256] = {0};
        for (int i = 0; i < 256; ++i)
        {
            hexMessage[i] = TwoChars2Hex(&inputChars[2 * i]);
        }
        SendHexMessage(hexMessage, sendToPort);
        std::cout << "Message Sent." << std::endl;
    }
    return 0;
}
