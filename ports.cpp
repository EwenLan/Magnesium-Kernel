#ifndef PORTS_HPP
#include "ports.hpp"
#endif
int* GetPorts()
{
    //TODO: Query free ports by using command line.
    static int ports[8] = {2334, 2335, 2336, 2337, 2338, 2339, 2340, 2341};
    return ports;
}
