#include "Socket.hpp"
#include <sys/socket.h>
#include <iostream>

using namespace Helper;

Socket::Socket(int pSocketDomain, int pSocketType, int pProtocol)
{
    int sockfd = socket(pSocketDomain, pSocketType, pProtocol);

    if (sockfd < 0)
    {
        throw std::runtime_error("Error: Failed to create socket!");
    }
    else
    {
        m_socketFileDescriptor = sockfd;
    }
}

Socket::~Socket()
{

}
