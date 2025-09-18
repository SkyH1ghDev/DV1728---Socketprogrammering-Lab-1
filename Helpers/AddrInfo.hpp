#pragma once

#include <netdb.h>
#include <string>

namespace Helper
{

    class AddrInfo
    {
    public:
        AddrInfo(const std::string& pHostname, const std::string& pService, const addrinfo* pHints);
        ~AddrInfo();

    public:
        addrinfo* GetAddrInfo() const;

    private:
        addrinfo* m_addressInfo;
    };

} // Helper
