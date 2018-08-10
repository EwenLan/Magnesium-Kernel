#include <iostream>

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
int main(int argc, char const *argv[])
{
    char c = 0;
    c = std::cin.get();
    while (c != '\n')
    {
        std::cout << Hex2String((unsigned char)c) << " ";
        c = std::cin.get();
    }
    std::cout << std::endl;
    return 0;
}
