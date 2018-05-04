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

enum module_type
{
    kernel = 1,
    ui = 3,
    camera = 5,
    nn = 7,
    payment = 9,
    database = 2
};

std::string ByteToHex(char c)
{
    std::string result;
    char a = (c >> 4) & 0x0f;
    char b = c & 0x0f;
    switch (a)
    {
    case 0x00:
        result += '0';
        break;
    case 0x01:
        result += '1';
        break;
    case 0x02:
        result += '2';
        break;
    case 0x03:
        result += '3';
        break;
    case 0x04:
        result += '4';
        break;
    case 0x05:
        result += '5';
        break;
    case 0x06:
        result += '6';
        break;
    case 0x07:
        result += '7';
        break;
    case 0x08:
        result += '8';
        break;
    case 0x09:
        result += '9';
        break;
    case 0x0a:
        result += 'a';
        break;
    case 0x0b:
        result += 'b';
        break;
    case 0x0c:
        result += 'c';
        break;
    case 0x0d:
        result += 'd';
        break;
    case 0x0e:
        result += 'e';
        break;
    case 0x0f:
        result += 'f';
        break;
    }
    switch (b)
    {
    case 0x00:
        result += '0';
        break;
    case 0x01:
        result += '1';
        break;
    case 0x02:
        result += '2';
        break;
    case 0x03:
        result += '3';
        break;
    case 0x04:
        result += '4';
        break;
    case 0x05:
        result += '5';
        break;
    case 0x06:
        result += '6';
        break;
    case 0x07:
        result += '7';
        break;
    case 0x08:
        result += '8';
        break;
    case 0x09:
        result += '9';
        break;
    case 0x0a:
        result += 'a';
        break;
    case 0x0b:
        result += 'b';
        break;
    case 0x0c:
        result += 'c';
        break;
    case 0x0d:
        result += 'd';
        break;
    case 0x0e:
        result += 'e';
        break;
    case 0x0f:
        result += 'f';
        break;
    }
    return result;
}

void SendMessage(int dest_sockfd, char *buffer, int message_length)
{
    int chars_left = message_length;
    int written_chars = 0;
    char *p = nullptr;
    while (chars_left)
    {
        written_chars = write(dest_sockfd, p, chars_left);
        chars_left -= written_chars;
        p += written_chars;
    }
}

void ShowRawMessage(char array[])
{
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            std::cout << ByteToHex(array[j + i * 16]) << "  ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    int instruction_loop = 1;
    int kernel_port = 0;
    int listen_port = 0;
    kernel_port = std::atoi(argv[1]);
    listen_port = std::atoi(argv[2]);

    int kernel_sockfd = 0;
    int self_sockfd = 0;
    struct sockaddr_in kernel_addr;
    struct sockaddr_in self_addr;

    kernel_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    kernel_addr.sin_family = AF_INET;
    kernel_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    kernel_addr.sin_port = htons(kernel_port);
    int kernel_sock_len = sizeof(kernel_addr);

    int connection_result = connect(kernel_sockfd, (struct sockaddr *)&kernel_addr, kernel_sock_len);
    if (connection_result == -1)
    {
        std::cout << "Cannot connect to kernel." << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Connect successfully." << std::endl;
    }
    std::cout << "Select simulation type: " << std::endl;
    int self_type = 0;
    std::cout << "1. Kernel. " << std::endl;
    std::cout << "2. Database. " << std::endl;
    std::cout << "3. User Interface. " << std::endl;
    std::cout << "5. Camera. " << std::endl;
    std::cout << "7. Neural Network. " << std::endl;
    std::cout << "9. Payment Module. " << std::endl;
    std::cout << ">> ";
    std::cin >> self_type;

    int send_to = 0;
    std::cout << "Messages will be send to: " << std::endl;
    std::cout << "1. Kernel. " << std::endl;
    std::cout << "2. Database. " << std::endl;
    std::cout << "3. User Interface. " << std::endl;
    std::cout << "5. Camera. " << std::endl;
    std::cout << "7. Neural Network. " << std::endl;
    std::cout << "9. Payment Module. " << std::endl;
    std::cout << ">> ";
    std::cin >> send_to;

    while (true)
    {
        std::string message_content;
        std::cout << "The message you want to send: ";
        std::cin >> message_content;
        if (message_content.size() > 252)
        {
            std::cout << "Error: The message's length exceed the limit. " << std::endl;
            std::cout << "This message will not be sent. Please enter again. " << std::endl;
            continue;
        }
        std::cout << "Current instruction loop: (auto)";
        std::string input;
        std::cin >> input;
        int send_instruction_loop = 0;
        if (input == "auto")
        {
            send_instruction_loop = instruction_loop;
        }
        else
        {
            send_instruction_loop = std::atoi(input.c_str());
        }
        char array_message[256] = {0};
        // ShowRawMessage(array_message);
        array_message[0] = (char)(self_type & 0x0000000f);
        array_message[1] = (char)(send_to & 0x0000000f);
        array_message[2] = (char)(send_instruction_loop & 0x0000ff00);
        array_message[3] = (char)(send_instruction_loop & 0x000000ff);
        // ShowRawMessage(array_message);
        // strcat(array_message, message_content.c_str());
        strcpy(&array_message[4], message_content.c_str());
        std::cout << "This message begin at: ";
        printf("%p, ", array_message);
        std::cout << "End at: ";
        printf("%p\n", &(array_message[256]));
        ShowRawMessage(array_message);
        ++instruction_loop;
    }
    return 0;
}