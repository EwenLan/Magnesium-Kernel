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
#ifndef _BITS_TIMEX_H
#include <time.h>
#endif
#ifndef MAIN_HPP
#include "main.hpp"
#endif

class session
{
  public:
    class fruitPicture
    {
      public:
        class mark
        {
          public:
            class point
            {
              public:
                unsigned int x;
                unsigned int y;
            };
            point top_left;
            point bottom_right;
            mark() = default;
            mark(unsigned int tlx, unsigned int tly, unsigned int brx, unsigned int bry)
            {
                this->top_left.x = tlx;
                this->top_left.y = tly;
                this->bottom_right.x = brx;
                this->bottom_right.y = bry;
            }
        };
        std::string picturePath;
        int weight;
        unsigned int code;
        std::vector<mark> fruitList;
        fruitPicture()
        {
            weight = 0;
            code = 0;
            picturePath = "";
        }
        fruitPicture(std::string path)
        {
            this->picturePath = path;
            weight = 0;
            code = 0;
        }
        void Add(unsigned int code, unsigned int tlx, unsigned int tly, unsigned int brx, unsigned int bry)
        {
            this->fruitList.push_back(mark(tlx, tly, brx, bry));
            this->code = code;
        }
    };
    class fruit
    {
      public:
        unsigned int code;
        int weight;
        fruit() = default;
        fruit(unsigned int code, int weight)
        {
            this->code = code;
            this->weight = weight;
        }
    };
    std::vector<fruitPicture> pictureList;
    std::vector<fruit> wholeFruitList;
    int sum_price;
    session() = default;
    int SearchCodeInWholeFruitList(unsigned int code)
    {
        for (int i = 0; i < wholeFruitList.size(); ++i)
        {
            if (code == wholeFruitList[i].code)
            {
                return i;
            }
        }
        return -1;
    }
    void Refresh()
    {
        wholeFruitList.clear();
        for (const auto &i : pictureList)
        {
            int searchResult = SearchCodeInWholeFruitList(i.code);
            if (searchResult != -1)
            {
                // Found it.
                wholeFruitList[searchResult].weight += i.weight;
            }
            else
            {
                // Not found it.
                wholeFruitList.push_back(fruit(i.code, i.weight));
            }
        }
    }
    int GetUnitPrice(unsigned int code)
    {
        // TODO: Read unit price from file.
        switch (code)
        {
        case 0x00010001:
            // Normal Red Apple
            return 1;
        case 0x00020001:
            // Normal Green Apple
            return 2;
        case 0x00030001:
            // Normal Pear
            return 3;
        default:
            return 4;
        }
    }
    int SumPrice()
    {
        Refresh();
        sum_price = 0;
        for (const auto &i : wholeFruitList)
        {
            sum_price += i.weight * GetUnitPrice(i.code);
        }
        return sum_price;
    }
};
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
char *GetModuleName(unsigned int module_code)
{
    static char module_name[16] = {0};
    switch (module_code)
    {
    case 1:
        strcpy(module_name, "Kernel");
        break;
    case 2:
        strcpy(module_name, "Database");
        break;
    case 3:
        strcpy(module_name, "Debugger");
        break;
    case 4:
        strcpy(module_name, "User Interface");
        break;
    case 5:
        strcpy(module_name, "Camera");
        break;
    case 6:
        strcpy(module_name, "Neural Network");
        break;
    case 7:
        strcpy(module_name, "Scale");
        break;
    case 8:
        strcpy(module_name, "Payment");
        break;
    case 9:
        strcpy(module_name, "Scanner");
        break;
    default:
        strcpy(module_name, "None");
        break;
    }
    return module_name;
}
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
void SendMessage(std::array<unsigned char, 252> &message, enum module_type to, int instruction_loop, std::array<int, 10> &port_table)
{
    unsigned char finalMessage[256] = {0};
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
void SendCommand(unsigned int command, enum module_type to, int instruction_loop, std::array<int, 10> &port_table)
{
    std::array<unsigned char, 252> sentMessage = {0};
    sentMessage[0] = (command & 0x0000ff00) >> 8;
    sentMessage[1] = (command & 0x000000ff);
    SendMessage(sentMessage, to, instruction_loop, port_table);
}
void KillByPid(int pid)
{
    // TODO: Kill a task by pid.
    std::cout << pid << " was killed. " << std::endl;
}
void FillArray(unsigned int value, std::array<unsigned char, 252> &sentMessage, int startPosition, int length)
{
    if (startPosition + length <= 252)
    {
        std::array<unsigned char, 4> valueArray = {0};
        for (int i = 0; i < 4; ++i)
        {
            valueArray[3 - i] = value & 0x000000ff;
            value = value >> 8;
        }
        for (int i = 0; i < length; ++i)
        {
            sentMessage[startPosition + i] = valueArray[i + 4 - length];
        }
    }
}
void HandleMessage(unsigned char *message, int local_instruction_loop, std::array<unsigned int, 10> &pid_table, std::array<int, 10> &port_table, session &current_session)
{
    std::array<unsigned char, 256> copiedInstruction;
    for (int i = 0; i < 256; ++i)
        copiedInstruction[i] = message[i];
    enum module_type from_module = (enum module_type)copiedInstruction[2];
    enum module_type to_module = (enum module_type)copiedInstruction[3];
    int recv_instruction_loop = (copiedInstruction[0] << 8) | copiedInstruction[1];
    int command = (copiedInstruction[4] << 8) | copiedInstruction[5];
    std::array<unsigned char, 250> argument = {0};
    std::array<unsigned char, 252> send_instruction = {0};
    for (int i = 0; i < 250; ++i)
    {
        argument[i] = copiedInstruction[6 + i];
    }
    // argument.fill(copiedInstruction[6]);
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
            std::cout << "|" << Hex2String(copiedInstruction[i * 16 + j]);
        }
        std::cout << std::endl;
    }
    if (to_module == kernel && recv_instruction_loop == local_instruction_loop)
    {
        std::cout << "Instruction Loop: ";
        std::cout << "Local Instruction Loop: " << local_instruction_loop << " ";
        std::cout << "Received Instruction Loop: " << recv_instruction_loop << std::endl;
        std::cout << "Sender: " << GetModuleName(from_module) << std::endl;
        std::cout << "Receiver: " << GetModuleName(to_module) << std::endl;
        std::cout << "Command: ";
        printf("%#06x\n", command);
        std::cout << "Argument(Integral): ";
        for (const auto &i : argument)
        {
            std::cout << Hex2String(i) << " ";
        }
        std::cout << std::endl;
        std::cout << "Argument(Char): ";
        for (const auto &i : argument)
        {
            std::cout << i;
        }
        std::cout << std::endl;
        std::array<unsigned char, 252> sentMessage = {0};
        std::string path;
        unsigned int code = 0;
        unsigned int tlx = 0;
        unsigned int tly = 0;
        unsigned int brx = 0;
        unsigned int bry = 0;
        unsigned int current_code = 0;
        int weight = 0;
        auto lastPicture = (current_session.pictureList.end() - 1);
        switch (command)
        {
        case 0x0001: // Report PID
            // Argument: 2 bytes unsigned integer value: PID.
            pid_table[from_module] = argument[0] << 8 | argument[1];
            std::cout << "PID Table updated. " << std::endl;
            std::cout << "PID Table: " << std::endl;
            for (int i = 1; i < 10; ++i)
            {
                std::cout << GetModuleName(i) << ": " << pid_table[i] << std::endl;
            }
            break;
        case 0x0002: // Report Error Infomation. Kernel will kill this module.
            // Argument: None.
            if (pid_table[from_module])
            {
                KillByPid(pid_table[from_module]);
                pid_table[from_module] = 0;
            }
            break;
        case 0x0401: // Message From User Interface: The user has press the "Start" button.
            // Kernel will send message to the camera. Camera will take a picture.
            // Argument: None
            SendCommand(0x0101, camera, local_instruction_loop, port_table);
            break;
        case 0x0501: // Message From Camera: Report the path of the picture.
            // Kernel will ask neural network for recognize the content of the picture.
            // Argument: The path of the picture
            for (auto &i : sentMessage)
                i = 0;
            sentMessage[0] = 0x01;
            sentMessage[1] = 0x03;
            for (int i = 0; i < 250; ++i)
            {
                sentMessage[i + 2] = argument[i];
            }
            SendMessage(sentMessage, nn, local_instruction_loop, port_table);
            for (const auto &i : argument)
            {
                path.push_back(i);
            }
            current_session.pictureList.push_back(session::fruitPicture(path));
            std::cout << "Picture paths in current session: " << std::endl;
            for (const auto &i : current_session.pictureList)
            {
                std::cout << i.picturePath << std::endl;
            }
            break;
        case 0x0601: // Message From Neural Network: Report the content of the picture.
            // Argument: 4 bytes unsigned integer value: fruit code.
            //           2 bytes
            //           2 bytes: top left corner's coordinate (x, y).
            //           2 bytes
            //           2 bytes: bottom right corner's coordinate (x, y).
            //           ...
            code = argument[0] << 24 | argument[1] << 16 | argument[2] << 8 | argument[3];
            for (int i = 0; i < 20; ++i)
            {
                current_code = argument[i * 12] << 24 | argument[i * 12 + 1] << 16 | argument[i * 12 + 2] << 8 | argument[i * 12 + 3];
                if (current_code != 0 && current_code != code)
                {
                    // Report various kinds of fruit error.
                    SendCommand(0x0106, ui, local_instruction_loop, port_table);
                    std::cout << "Error: Various kinds of fruits on the scale." << std::endl;
                    if (current_session.pictureList.end() != current_session.pictureList.begin())
                    {
                        auto delElement = current_session.pictureList.end() - 1;
                        current_session.pictureList.erase(delElement);
                    }
                    return;
                }
            }
            (current_session.pictureList.end() - 1)->code = code;
            printf("Info: Fruits %#010x in picture %s: \n", code, (current_session.pictureList.end() - 1)->picturePath.c_str());
            for (int i = 0; i < 20; ++i)
            {
                current_code = argument[i * 12] << 24 | argument[i * 12 + 1] << 16 | argument[i * 12 + 2] << 8 | argument[i * 12 + 3];
                if (current_code)
                {
                    tlx = argument[i * 12 + 4] << 8 | argument[i * 12 + 5];
                    tly = argument[i * 12 + 6] << 8 | argument[i * 12 + 7];
                    brx = argument[i * 12 + 8] << 8 | argument[i * 12 + 9];
                    bry = argument[i * 12 + 10] << 8 | argument[i * 12 + 11];
                    (current_session.pictureList.end() - 1)->fruitList.push_back(session::fruitPicture::mark(tlx, tly, brx, bry));
                    printf("Area: (%5d, %5d) - (%5d, %5d)\n", tlx, tly, brx, bry);
                }
            }
            SendCommand(0x0102, scale, local_instruction_loop, port_table); // Send weighing request.
            break;
        case 0x0701: // Message From Scale: Report the weight of the fruits.
            // Kernel will send the code, weight and sum price to the user interface.
            // Argument: 4 bytes unsigned integer value: weight(gram).
            weight = argument[0] << 24 | argument[1] << 16 | argument[2] << 8 | argument[3];
            (current_session.pictureList.end() - 1)->weight = weight;
            FillArray(0x0107, sentMessage, 0, 2);
            FillArray(lastPicture->code, sentMessage, 2, 4);
            FillArray(weight, sentMessage, 6, 4);
            FillArray(current_session.GetUnitPrice(lastPicture->code) * lastPicture->weight, sentMessage, 10, 4);
            SendMessage(sentMessage, ui, local_instruction_loop, port_table);
            // Stop Here -- Ewen Lan Aug 9 2018
            break;
        }
    }
    else
    {
        if (to_module != kernel)
        {
            std::cout << "Warning: Receiver is not kernel. " << std::endl;
        }
        if (recv_instruction_loop != local_instruction_loop)
        {
            std::cout << "Error: Received instruction loop not match. " << std::endl;
        }
    }
}
int main(int argc, char *argv[])
{
    std::array<int, 10> status_table = {0};
    std::array<unsigned int, 10> pid_table = {0};
    // unsigned int pid_table[10] = {0};
    for (int i = 1; i < 10; ++i)
    {
        pid_table[i] = 0x00;
    }
    int instruction_loop = 1;
    // std::vector<goods> shopping_list;
    std::array<int, 10> port_table = {0};
    int current_goods_code = 0;
    unsigned int current_weight = 0;
    //TODO: Generate port_table automatically.
    port_table[1] = 2334; // Kernel
    port_table[2] = 2335; // Database
    port_table[3] = 2336; // Debugger
    port_table[4] = 2337; // User Interface
    port_table[5] = 2338; // Camera
    port_table[6] = 2339; // Neural Network
    port_table[7] = 2340; // Scale
    port_table[8] = 2341; // Payment
    port_table[9] = 2342; // Scanner
    std::array<std::thread, 10> thread_table;
    for (auto &i : status_table)
        i = -1;
    // for (auto &i : pid_table)
    //     i = -1;
    // thread_table[debugger] = std::thread(RunModule, debugger, port_table[kernel], port_table[debugger]);
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
    std::cout << "Kernel started. " << std::endl;
    session current_session;
    while (true)
    {
        client_socketfd = accept(kernel_socketfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_socket_len);
        time_t current_time = time(NULL);
        char *c_time_string = ctime(&current_time);
        std::cout << "Received a message at " << c_time_string << std::endl;
        unsigned char buffer[256] = {0};
        int n = read(client_socketfd, buffer, 256);
        close(client_socketfd);
        // auto messageHandler = std::thread(HandleMessage, std::ref(message), instruction_loop, std::ref(pid_table), std::ref(port_table));
        HandleMessage(buffer, instruction_loop, pid_table, port_table, current_session);
    }
}