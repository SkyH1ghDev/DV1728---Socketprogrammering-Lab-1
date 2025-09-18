#pragma once
#include <sys/socket.h>

namespace Helper
{
    class Socket
    {
    public:
        Socket() = default;
        Socket(int pSocketDomain, int pSocketType, int pProtocol);
        Socket& operator=(const Socket& other) = default;
        ~Socket();

    private:
        int m_socketFileDescriptor = 0;
    };

} // Helper