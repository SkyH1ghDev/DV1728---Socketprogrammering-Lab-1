#include "Socket.hpp"
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>

using namespace Helper;

Socket::Socket(int pSocketDomain, int pSocketType, int pProtocol, addrinfo* pCurrentAddress)
{
    int sockfd = socket(pSocketDomain, pSocketType, pProtocol);

    if (sockfd < 0)
    {
        throw std::exception();
    }

    m_socketFileDescriptor = sockfd;
    m_currentAddress = pCurrentAddress;

    Connect(pCurrentAddress);

    timeval timeoutTime{};
    timeoutTime.tv_sec = 2;
    timeoutTime.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeoutTime, sizeof(timeoutTime));
}

Socket::Socket(Socket&& other) noexcept
{
    std::swap(m_socketFileDescriptor, other.m_socketFileDescriptor);
    std::swap(m_currentAddress, other.m_currentAddress);
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(m_socketFileDescriptor, other.m_socketFileDescriptor);
    std::swap(m_currentAddress, other.m_currentAddress);

    return *this;
}

Socket::~Socket()
{
    if (m_socketFileDescriptor != 0)
    {
        close(m_socketFileDescriptor);
    }
}

addrinfo* Socket::GetAddressData() const
{
    return m_currentAddress;
}

void Socket::Connect(const addrinfo* pAddressInformation) const
{
    int connection = connect(m_socketFileDescriptor, pAddressInformation->ai_addr, pAddressInformation->ai_addrlen);

    if (connection < 0)
    {
        std::cerr << "Error: Failed to connect!\n";
        throw std::runtime_error("Testing whether I get here in tests");
    }
}

void Socket::SendBinary(calcMessage pCalcMessage, int pFlags) const
{
    pCalcMessage.type = htons(pCalcMessage.type);
    pCalcMessage.message = htonl(pCalcMessage.message);
    pCalcMessage.protocol = htons(pCalcMessage.protocol);
    pCalcMessage.major_version = htons(pCalcMessage.major_version);
    pCalcMessage.minor_version = htons(pCalcMessage.minor_version);

    std::vector<std::byte> payload(sizeof pCalcMessage);
    std::memcpy(payload.data(), &pCalcMessage, sizeof pCalcMessage);

    std::intmax_t bytesSent = send(m_socketFileDescriptor, payload.data(), payload.size(), pFlags);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error: Failed to send bytes\n");
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";
}

void Socket::SendBinary(calcProtocol pCalcProtocol, int pFlags) const
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

    std::intmax_t bytesSent = send(m_socketFileDescriptor, payload.data(), payload.size(), pFlags);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error: Failed to send bytes\n");
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";
}

void Socket::SendText(const std::string& pMessage, int pFlags) const
{
    std::intmax_t bytesSent = send(m_socketFileDescriptor, pMessage.data(), pMessage.size(), pFlags);

    if (bytesSent < 0)
    {
        throw std::runtime_error("Error: Failed to send bytes\n");
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";
}

std::variant<calcMessage, calcProtocol> Socket::ReceiveBinary(int pFlags) const
{
    std::vector<std::byte> buffer(sizeof(calcProtocol));
    std::intmax_t bytesReceived = recv(m_socketFileDescriptor, buffer.data(), buffer.size(), pFlags);

    if (bytesReceived < 0)
    {
        std::cerr << "ERROR: MESSAGE LOST (TIMEOUT)\n";
        exit(EXIT_FAILURE);
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

std::string Socket::ReceiveText(int pFlags) const
{
    std::vector<char> buffer(4096);
    std::vector<pollfd> pollFileDescriptors =
    {
        {m_socketFileDescriptor, POLLIN}
    };

    poll(pollFileDescriptors.data(), pollFileDescriptors.size(), 2000);

    if (pollFileDescriptors.at(0).revents & POLLIN)
    {
        int bytesRead = recv(m_socketFileDescriptor, buffer.data(), buffer.size(), pFlags);
    }

    return { buffer.data() };
}
