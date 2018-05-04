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

extern int *GetPorts();
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
    kernel = 1,
    ui = 3,
    camera = 5,
    nn = 7,
    payment = 9,
    database = 2
};
enum message_type
{
    pid,
    module_error,
    start,
    cancel,
    quit,
    img_path,
    take_photo,
    img_content,
    pay_success,
    pay_fail
};
void RunModule(const char command[])
{
    int failed_times = 0;
    int returnValue = 1;
    do
    {
        returnValue = system(command);
        failed_times += 1;
    } while (returnValue != 0 && failed_times < 4);
}
int GetPID(std::string message)
{
    int x = message.find("PID");
    int pid = -1;
    if (x == 0)
    {
        pid = std::stoi(message.substr(4, 5), nullptr, 10);
    }
    return pid;
}
int GetMessageType(std::string message)
{
    if (message.find_first_of("PID"))
        return pid;
    if (message.find_first_of("ERROR"))
        return error;
    if (message.find_first_of("START"))
        return start;
    if (message.find_first_of("CANCEL"))
        return cancel;
    if (message.find_first_of("QUIT"))
        return quit;
    if (message.find_first_of("PATH"))
        return img_path;
    if (message.find_first_of("TAKE_PHOTO"))
        return take_photo;
    if (message.find_first_of("CONTENT"))
        return img_content;
    return -1;
}
bool IsLegalInstruction(enum module_type mod, enum message_type msg)
{
    char law[10][10];
    //  pid            error          start          cancel         quit           img_path       take_photo     img_content    pay_success    pay_fail
    law[0][0] = 0;
    law[0][1] = 0;
    law[0][2] = 0;
    law[0][3] = 0;
    law[0][4] = 0;
    law[0][5] = 0;
    law[0][6] = 0;
    law[0][7] = 0;
    law[0][8] = 0;
    law[0][9] = 0; //none
    law[1][0] = 1;
    law[1][1] = 1;
    law[1][2] = 1;
    law[1][3] = 1;
    law[1][4] = 1;
    law[1][5] = 1;
    law[1][6] = 1;
    law[1][7] = 1;
    law[1][8] = 1;
    law[1][9] = 1; //kernel
    law[2][0] = 1;
    law[2][1] = 1;
    law[2][2] = 1;
    law[2][3] = 1;
    law[2][4] = 1;
    law[2][5] = 1;
    law[2][6] = 1;
    law[2][7] = 1;
    law[2][8] = 1;
    law[2][9] = 1; //database
    law[3][0] = 0;
    law[3][1] = 0;
    law[3][2] = 0;
    law[3][3] = 0;
    law[3][4] = 0;
    law[3][5] = 0;
    law[3][6] = 0;
    law[3][7] = 0;
    law[3][8] = 0;
    law[3][9] = 0; //ui
    law[4][0] = 0;
    law[4][1] = 0;
    law[4][2] = 0;
    law[4][3] = 0;
    law[4][4] = 0;
    law[4][5] = 0;
    law[4][6] = 0;
    law[4][7] = 0;
    law[4][8] = 0;
    law[4][9] = 0; //none
    law[5][0] = 0;
    law[5][1] = 0;
    law[5][2] = 0;
    law[5][3] = 0;
    law[5][4] = 0;
    law[5][5] = 0;
    law[5][6] = 0;
    law[5][7] = 0;
    law[5][8] = 0;
    law[5][9] = 0; //camera
    law[6][0] = 0;
    law[6][1] = 0;
    law[6][2] = 0;
    law[6][3] = 0;
    law[6][4] = 0;
    law[6][5] = 0;
    law[6][6] = 0;
    law[6][7] = 0;
    law[6][8] = 0;
    law[6][9] = 0; //none
    law[7][0] = 0;
    law[7][1] = 0;
    law[7][2] = 0;
    law[7][3] = 0;
    law[7][4] = 0;
    law[7][5] = 0;
    law[7][6] = 0;
    law[7][7] = 0;
    law[7][8] = 0;
    law[7][9] = 0; //nn
    law[8][0] = 0;
    law[8][1] = 0;
    law[8][2] = 0;
    law[8][3] = 0;
    law[8][4] = 0;
    law[8][5] = 0;
    law[8][6] = 0;
    law[8][7] = 0;
    law[8][8] = 0;
    law[8][9] = 0; //none
    law[9][0] = 0;
    law[9][1] = 0;
    law[9][2] = 0;
    law[9][3] = 0;
    law[9][4] = 0;
    law[9][5] = 0;
    law[9][6] = 0;
    law[9][7] = 0;
    law[9][8] = 0;
    law[9][9] = 0; //payment
    if (mod >= 0 && mod <= 9 && msg >= 0 && msg <= 9)
        return law[mod][msg];
    return 0;
}
int main(int argc, char *argv[])
{
    volatile char status_table[10] = {0};
    volatile int pid_table[10] = {0};
    volatile int instruction_loop = 1;
    for (auto &i : status_table)
        i = -1;
    for (auto &i : pid_table)
        i = -1;
    std::thread testSubTask(RunModule, "./simulator 2334 2335"); // module: database
    //TODO: Run all subtasks.
    status_table[database] = started;
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
        char buffer[257] = {0};
        int n = read(client_socketfd, buffer, 256);
        enum module_type from;
        enum module_type to;
        int recv_instruction_loop = buffer[0] << 8 | buffer[1];
        from = (enum module_type)buffer[2];
        to = (enum module_type)buffer[3];
        std::string message = &buffer[4];
        int pid = GetPID(message);
        if (pid > 1)
        {
            status_table[from] = standing_by;
        }
        else
        {
        }
        std::cout << "pid: " << pid << std::endl;
        send(client_socketfd, "received.", 9, 0);
    }
    testSubTask.join();
}