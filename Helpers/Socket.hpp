#pragma once
#include <netdb.h>
#include <protocol.h>
#include <variant>

namespace Helper
{
    class Socket
    {
    public:
        Socket() = default;
        Socket(Socket&& other) noexcept;
        Socket& operator=(Socket&& other) noexcept;
        Socket(Socket& other) = delete;
        Socket& operator=(Socket& other) = delete;
        Socket(int pSocketDomain, int pSocketType, int pProtocol);

        ~Socket();

    public:
        //void Send(const std::string& buffer, int flags);
        void SendTo(calcMessage pCalcMessage, int pFlags, const addrinfo* pAddressInformation);
        void SendTo(calcProtocol pCalcProtocol, int pFlags, const addrinfo* pAddressInformation);
        //void SendMessage(const msghdr* msg, int flags);

        //void Receive(const std::string& buffer, int flags);
        std::variant<calcMessage, calcProtocol> ReceiveFrom(int pFlags, addrinfo* pAddressInformation);
        //void ReceiveMessage();


    private:
        int m_socketFileDescriptor = 0;

    };

}