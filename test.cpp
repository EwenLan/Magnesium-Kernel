#include <iostream>

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

int main(int argc, char *argv[])
{
    char a = 0;
    while (std::cin >> a)
    {
        std::cout << ByteToHex(a) << std::endl;
    }
    return 0;
}