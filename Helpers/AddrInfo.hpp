#pragma once

#include <netdb.h>
#include <string>

namespace Helper
{

    class AddrInfo
    {
    public:
        AddrInfo() = default;
        AddrInfo(AddrInfo&& other) noexcept;
        AddrInfo& operator=(AddrInfo&& other) noexcept;
        AddrInfo(AddrInfo& other) = delete;
        AddrInfo& operator=(AddrInfo& other) = delete;
        AddrInfo(const std::string& pHostname, const std::string& pService, const addrinfo& pHints);
        ~AddrInfo();

    public:
        addrinfo* GetAddrInfo() const;

    private:
        addrinfo* m_addressInfo = nullptr;
    };

} // Helper
