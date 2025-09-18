#include "AddrInfo.hpp"

using namespace Helper;

AddrInfo::AddrInfo(const std::string& pHostname, const std::string& pService, const addrinfo* pHints)
{
    getaddrinfo(pHostname.c_str(), pService.c_str(), pHints, &m_addressInfo);
}

AddrInfo::~AddrInfo()
{
    freeaddrinfo(m_addressInfo);
}

addrinfo* AddrInfo::GetAddrInfo() const
{
    return m_addressInfo;
}
