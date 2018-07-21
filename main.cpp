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
#ifndef MAIN_HPP
#include "main.hpp"
#endif

// extern int *GetPorts();
class goods
{
  public:
    int goods_code;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int weight;
    int unit_price;
    int sum_price;
    goods(int goods_code, int xmin, int ymin, int xmax, int ymax);
    void caculatePrice()
    {
        // TODO: Read unit price from file.
        switch (goods_code)
        {
        case 0x0101:
            unit_price = 1;
            break;
        case 0x0201:
            unit_price = 1;
            break;
        case 0x0301:
            unit_price = 1;
            break;
        default:
            unit_price = 1;
        }
        sum_price = unit_price * weight;
    }
};
goods::goods(int goods_code, int xmin, int ymin, int xmax, int ymax)
{
    this->goods_code = goods_code;
    this->xmin = xmin;
    this->ymin = ymin;
    this->xmax = xmax;
    this->ymax = ymax;
}
enum status_type
{
    standing_by,
    error,
    busy,
    started,
    unavaliable = -1
};
enum module_type
{
    kernel = 0x01,
    database = 0x02,
    debugger = 0x03,
    ui = 0x04,
    camera = 0x05,
    nn = 0x06,
    scale = 0x07,
    payment = 0x08,
    scanner = 0x09
};
void RunModule(int moduleIndex, int kernel_port, int module_port)
{
    std::array<std::string, 10> module_path_table;
    //TODO: Add every module in this table
    module_path_table[3] = "./simulator";
    char wholeCmd[256] = {0};
    sprintf(wholeCmd, "%s %d %d", module_path_table[moduleIndex].c_str(), kernel_port, module_port);
    int triedTimes = 0;
    int returnValue = 1;
    do
    {
        returnValue = system(wholeCmd);
        ++triedTimes;
    } while (triedTimes < 3 && returnValue != 0);
    //TODO: Deal with module fail.
}
void string2array(std::string str, std::array<char, 252> dest_array)
{
    for (int i = 0; i < (str.size() > 252 ? 252 : str.size()); ++i)
    {
        dest_array[i] = str[i];
    }
}
void SendMessage(std::array<char, 252> message, enum module_type to, int instruction_loop, std::array<int, 10> port_table)
{
    char finalMessage[256] = {0};
    for (int i = 0; i < (message.size() > 252 ? 252 : message.size()); ++i)
    {
        finalMessage[4 + i] = message[i];
    }
    finalMessage[0] = (instruction_loop & 0x0000ff00) >> 8;
    finalMessage[1] = instruction_loop & 0x000000ff;
    finalMessage[2] = 0x01;
    finalMessage[3] = (int)to;
    int module_sockfd = 0;
    int moduel_len = 0;
    struct sockaddr_in module_addr;
    module_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    module_addr.sin_family = AF_INET;
    module_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    module_addr.sin_port = htons(port_table[to]);
    moduel_len = sizeof(module_addr);
    int result = connect(module_sockfd, (struct sockaddr *)&module_addr, moduel_len);
    // TODO: Deal with connection fail.
    write(module_sockfd, finalMessage, 256);
    close(module_sockfd);
}
void KillByPid(int pid)
{
    // TODO: Kill a task by pid.
}
void HandleMessage(std::array<char, 256> instruction, int local_instruction_loop, std::array<unsigned int, 10> pid_table, std::array<int, 10> port_table, std::vector<goods> shopping_list, int &current_goods_code, unsigned int &current_goods_weight, int &current_goods_xmin, int &current_goods_ymin, int &current_goods_xmax, int &current_goods_ymax)
{
    std::array<char, 256> copiedInstruction(instruction);
    enum module_type from_module = (enum module_type)copiedInstruction[2];
    enum module_type to_module = (enum module_type)copiedInstruction[3];
    int recv_instruction_loop = (copiedInstruction[0] << 8) | copiedInstruction[1];
    int command = (copiedInstruction[4] << 8) | copiedInstruction[5];
    std::array<char, 250> argument = {0};
    std::array<char, 252> send_instruction = {0};
    for (int i = 0; i < 250; ++i)
    {
        argument[i] = copiedInstruction[6 + i];
    }
    if (to_module == kernel && recv_instruction_loop == local_instruction_loop)
    {
        if (command == 0x0001)
        {
            // pid_table[from_module] = (argument[0] << 24) | (argument[1] << 16) | (argument[2] << 8) | argument[3];
            pid_table[from_module] = (argument[0] << 8) | argument[1];
        }
        else if (command == 0x0002)
        {
            KillByPid(pid_table[from_module]);
        }
        else if (command == 0x0401)
        {
            send_instruction[0] = 0x01;
            send_instruction[1] = 0x01;
            std::thread sendInstructionToCamera(SendMessage, send_instruction, camera, local_instruction_loop, port_table);
            send_instruction[1] = 0x02;
            std::thread sendInstructionToScale(SendMessage, send_instruction, scale, local_instruction_loop, port_table);
            sendInstructionToCamera.join();
            sendInstructionToScale.join();
        }
        else if (command == 0x0701)
        {
            if (current_goods_code != 0)
            {
                shopping_list.push_back(goods(current_goods_code, current_goods_xmin, current_goods_ymin, current_goods_xmax, current_goods_ymax));
                shopping_list[shopping_list.size() - 1].caculatePrice();
                // TODO: Send message to ui. -- June 17 2018 Ewen Lan
                current_goods_code = 0;
                current_goods_weight = 0;
            }
            else
            {
                current_goods_weight = (argument[0] << 24) | (argument[1] << 16) | (argument[2] << 8) | argument[3];
            }
        }
        else if (command == 0x0601)
        {
            current_goods_code = (argument[0] << 8) | argument[1];
            current_goods_xmin = (argument[2] << 8) | argument[3];
            current_goods_ymin = (argument[4] << 8) | argument[5];
            current_goods_xmax = (argument[6] << 8) | argument[7];
            current_goods_ymax = (argument[8] << 8) | argument[9];
            if (current_goods_weight != 0)
            {
                shopping_list.push_back(goods(current_goods_code, current_goods_xmin, current_goods_ymin, current_goods_xmax, current_goods_ymax));
                shopping_list[shopping_list.size() - 1].caculatePrice();
                current_goods_code = 0;
                current_goods_weight = 0;
            }
        }
    }
}
int main(int argc, char *argv[])
{
    std::array<int, 10> status_table = {0};
    std::array<unsigned int, 10> pid_table = {0};
    volatile int instruction_loop = 1;
    std::vector<goods> shopping_list;
    std::array<int, 10> port_table = {0};
    int current_goods_code = 0;
    unsigned int current_weight = 0;
    //TODO: Generate port_table automatically.
    port_table[1] = 2334;
    port_table[2] = 2335;
    port_table[3] = 2336;
    port_table[4] = 2337;
    port_table[5] = 2338;
    port_table[6] = 2339;
    port_table[7] = 2340;
    port_table[8] = 2341;
    port_table[9] = 2342;
    std::array<std::thread, 10> thread_table;
    for (auto &i : status_table)
        i = -1;
    for (auto &i : pid_table)
        i = -1;
    thread_table[debugger] = std::thread(RunModule, debugger, port_table[kernel], port_table[debugger]);
    //TODO: Run all subtasks.
    status_table[debugger] = started;
    //TODO: Set status for each modules.

    int kernel_socketfd = 0;
    int client_socketfd = 0;
    int kernel_socket_len = 0;
    int client_socket_len = 0;
    struct sockaddr_in kernel_addr;
    struct sockaddr_in client_addr;
    kernel_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    kernel_addr.sin_family = AF_INET;
    kernel_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    kernel_addr.sin_port = htons(2334);
    kernel_socket_len = sizeof(kernel_addr);
    bind(kernel_socketfd, (struct sockaddr *)&kernel_addr, kernel_socket_len);
    listen(kernel_socketfd, 6);
    client_socket_len = sizeof(client_addr);
    while (true)
    {
        client_socketfd = accept(kernel_socketfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_socket_len);
        char buffer[256] = {0};
        int n = read(client_socketfd, buffer, 256);
        close(client_socketfd);
        enum module_type from;
        enum module_type to;
        int recv_instruction_loop = buffer[0] << 8 | buffer[1];
        from = (enum module_type)buffer[2];
        to = (enum module_type)buffer[3];
        std::array<char, 252> message;
        for (int i = 0; i < 252; ++i)
        {
            message[i] = buffer[i + 4];
        }
    }
}