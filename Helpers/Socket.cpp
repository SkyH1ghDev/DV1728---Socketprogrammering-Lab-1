#include "Socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>

using namespace Helper;

Socket::Socket(int pSocketDomain, int pSocketType, int pProtocol)
{
    int sockfd = socket(pSocketDomain, pSocketType, pProtocol);

    if (sockfd < 0)
    {
        throw std::exception();
    }

    m_socketFileDescriptor = sockfd;

    timeval timeoutTime{};
    timeoutTime.tv_sec = 2;
    timeoutTime.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeoutTime, sizeof(timeoutTime));
}

Socket::Socket(Socket&& other) noexcept
{
    std::swap(m_socketFileDescriptor, other.m_socketFileDescriptor);
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(m_socketFileDescriptor, other.m_socketFileDescriptor);

    return *this;
}

Socket::~Socket()
{
    if (m_socketFileDescriptor != 0)
    {
        close(m_socketFileDescriptor);
    }
}

void Socket::SendToBinary(calcMessage pCalcMessage, int pFlags, const addrinfo* pAddressInformation)
{
    pCalcMessage.type = htons(pCalcMessage.type);
    pCalcMessage.message = htonl(pCalcMessage.message);
    pCalcMessage.protocol = htons(pCalcMessage.protocol);
    pCalcMessage.major_version = htons(pCalcMessage.major_version);
    pCalcMessage.minor_version = htons(pCalcMessage.minor_version);

    std::vector<std::byte> payload(sizeof pCalcMessage);
    std::memcpy(payload.data(), &pCalcMessage, sizeof pCalcMessage);

    std::intmax_t bytesSent = sendto(m_socketFileDescriptor, payload.data(), payload.size(), pFlags, pAddressInformation->ai_addr, pAddressInformation->ai_addrlen);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error: Failed to send bytes\n");
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";
}

void Socket::SendToBinary(calcProtocol pCalcProtocol, int pFlags, const addrinfo* pAddressInformation)
{
    pCalcProtocol.type = htons(pCalcProtocol.type);
    pCalcProtocol.major_version = htons(pCalcProtocol.major_version);
    pCalcProtocol.minor_version = htons(pCalcProtocol.minor_version);
    pCalcProtocol.id = htonl(pCalcProtocol.id);
    pCalcProtocol.arith = htonl(pCalcProtocol.arith);
    pCalcProtocol.inValue1 = htonl(pCalcProtocol.inValue1);
    pCalcProtocol.inValue2 = htonl(pCalcProtocol.inValue2);
    pCalcProtocol.inResult = htonl(pCalcProtocol.inResult);

    std::vector<std::byte> payload(sizeof pCalcProtocol);
    std::memcpy(payload.data(), &pCalcProtocol, sizeof pCalcProtocol);

    std::intmax_t bytesSent = sendto(m_socketFileDescriptor, payload.data(), payload.size(), pFlags, pAddressInformation->ai_addr, pAddressInformation->ai_addrlen);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error: Failed to send bytes\n");
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";
}


std::variant<calcMessage, calcProtocol> Socket::ReceiveFromBinary(int pFlags, addrinfo* pAddressInformation)
{
    std::vector<std::byte> buffer(sizeof(calcProtocol));
    std::intmax_t bytesReceived = recvfrom(m_socketFileDescriptor, buffer.data(), buffer.size(), pFlags, pAddressInformation->ai_addr, &pAddressInformation->ai_addrlen);

    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error: Timed out. Failed to receive bytes.\n");
    }

    if (bytesReceived == sizeof(calcMessage))
    {
        calcMessage message{};
        std::memcpy(&message, buffer.data(), sizeof(message));
        message.type = ntohs(message.type);
        message.message = ntohl(message.message);
        message.protocol = ntohs(message.protocol);
        message.major_version = ntohs(message.major_version);
        message.minor_version = ntohs(message.minor_version);

         if (message.message == 2)
         {
             throw std::runtime_error("Error: Server sent REJECT message\n");
         }

         return message;

    }
    if (bytesReceived == sizeof(calcProtocol))
    {
        calcProtocol response{};
        std::memcpy(&response, buffer.data(), sizeof(response));
        response.type = ntohs(response.type);
        response.major_version = ntohs(response.major_version);
        response.minor_version = ntohs(response.minor_version);
        response.id = ntohl(response.id);
        response.arith = ntohl(response.arith);
        response.inValue1 = ntohl(response.inValue1);
        response.inValue2 = ntohl(response.inValue2);
        response.inResult = ntohl(response.inResult);

        return response;
    }

    throw std::runtime_error("Error: Wrong packet size or incorrect protocol\n");
}