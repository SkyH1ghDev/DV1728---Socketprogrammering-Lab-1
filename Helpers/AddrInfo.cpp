#include "AddrInfo.hpp"
#include <iostream>
#include <algorithm>

using namespace Helper;

AddrInfo::AddrInfo(const std::string& pHostname, const std::string& pService, const addrinfo& pHints)
{
    if (getaddrinfo(pHostname.c_str(), pService.c_str(), &pHints, &m_addressInfo) != 0)
    {
        throw std::runtime_error("Error: Failed to create server information");
    }
}

AddrInfo::AddrInfo(AddrInfo&& other) noexcept
{
    std::swap(m_addressInfo, other.m_addressInfo);
}

AddrInfo& AddrInfo::operator=(AddrInfo&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(m_addressInfo, other.m_addressInfo);

    return *this;
}


AddrInfo::~AddrInfo()
{
    if (m_addressInfo != nullptr)
    {
        freeaddrinfo(m_addressInfo);
    }
}

addrinfo* AddrInfo::GetAddrInfo() const
{
    return m_addressInfo;
}
