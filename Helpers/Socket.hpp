#pragma once
#include <netdb.h>
#include <protocol.h>
#include <variant>
#include <string>

namespace Helper
{
    class Socket
    {
    public:
        Socket() = default;
        Socket(int pSocketDomain, int pSocketType, int pProtocol, addrinfo* pCurrentAddress);
        Socket(Socket& other) = delete;
        Socket& operator=(Socket& other) = delete;
        Socket(Socket&& other) noexcept;
        Socket& operator=(Socket&& other) noexcept;

        ~Socket();

    public:
        addrinfo* GetAddressData() const;

        void Connect(const addrinfo* pAddressInformation) const;

        void SendBinary(calcMessage pCalcMessage, int pFlags) const;
        void SendBinary(calcProtocol pCalcProtocol, int pFlags) const;
        void SendText(const std::string& pMessage, int pFlags) const;
        //void SendText(calcMessage pCalcProtocol, int pFlags) const;
        //void SendText(calcProtocol pCalcProtocol, int pFlags) const;

        std::variant<calcMessage, calcProtocol> ReceiveBinary(int pFlags) const;
        std::string ReceiveText(int pFlags)const ;



    private:
        int m_socketFileDescriptor = 0;
        addrinfo* m_currentAddress = nullptr;
    };
}
