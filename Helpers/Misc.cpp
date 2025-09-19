#include "Misc.hpp"
#include <iostream>
#include <vector>
#include <sstream>

using namespace Helper;

void Misc::CheckPortValidity(int pPort)
{
    if (pPort < 1 or pPort > 65535)
    {
        std::cerr << "Error: Port is out of server scope.\n";
        if (pPort > 65535)
        {
            std::cerr << "Error: Port is not a valid UDP or TCP port.\n";
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
}

Socket Misc::CreateSocket(const AddrInfo& pServerInformation)
{
    addrinfo* addrInfo = nullptr;
    Socket socket;
    for (addrInfo = pServerInformation.GetAddrInfo(); addrInfo != nullptr; addrInfo = addrInfo->ai_next)
    {
        try
        {
            socket = std::move(Socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol, addrInfo));
        } catch (std::exception)
        {
            continue;
        }

        break;
    }

    if (addrInfo == nullptr)
    {
        std::cerr << "Error: Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    return socket;
}

int Misc::PerformArithmetic(int pOperand, int pVal1, int pVal2)
{
    int result = 0;
    switch (pOperand)
    {
        case 1:
            result = pVal1 + pVal2;
            std::cout << "ASSIGNMENT: add " << pVal1 << " " << pVal2 << "\n";
            break;

        case 2:
            result = pVal1 - pVal2;
            std::cout << "ASSIGNMENT: sub " << pVal1 << " " << pVal2 << "\n";
            break;

        case 3:
            result = pVal1 * pVal2;
            std::cout << "ASSIGNMENT: mul " << pVal1 << " " << pVal2 << "\n";
            break;

        case 4:
            result = pVal1 / pVal2;
            std::cout << "ASSIGNMENT: div " << pVal1 << " " << pVal2 << "\n";
            break;

        default:
            throw std::runtime_error("Error: Undefined arithmetic operator-value received\n");
    }

    return result;
}


void Misc::PerformBinaryCommunication(const Socket& pSocket)
{
    calcMessage firstMessage{};
    firstMessage.type = 22;
    firstMessage.message = 0;
    firstMessage.protocol = 17;
    firstMessage.major_version = 1;
    firstMessage.minor_version = 1;

    // UDP
    if (pSocket.GetAddressData()->ai_socktype == SOCK_DGRAM)
    {
        pSocket.SendBinary(firstMessage, 0);
    }

    // TCP
    if (pSocket.GetAddressData()->ai_socktype == SOCK_STREAM)
    {
        std::string response = pSocket.ReceiveText(0);
        std::stringstream ss(response);
        std::vector<std::string> tokens;
        std::string temp;

        while (getline(ss, temp, '\n'))
        {
            tokens.push_back(temp);
        }

        for (std::string& token : tokens)
        {
            if (token == "BINARY TCP 1.1")
            {
                break;
            }

            if (&token == &tokens.back())
            {
                std::cout << "ERROR: MISSMATCH PROTOCOL\n";
                exit(EXIT_FAILURE);
            };
        }

        pSocket.SendText("BINARY TCP 1.1 OK\n", 0);
    }

    std::variant<calcMessage, calcProtocol> responseVariant = pSocket.ReceiveBinary(0);
    calcProtocol response = std::get<calcProtocol>(responseVariant);

    response.inResult = PerformArithmetic(response.arith, response.inValue1, response.inValue2);

    response.type = 2;
    pSocket.SendBinary(response, 0);
    responseVariant = pSocket.ReceiveBinary(0);
    calcMessage test2 = std::get<calcMessage>(responseVariant);

    if (test2.message != 1)
    {
        std::cout << "ERROR: Server sent error\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "OK (myresult=" << response.inResult << ")\n";
}

void Misc::PerformTextCommunication(const Socket& pSocket)
{
    std::string response;
    std::vector<std::string> tokens;
    std::string temp;
    std::stringstream ss;

    // UDP

    if (pSocket.GetAddressData()->ai_socktype == SOCK_DGRAM)
    {
        pSocket.SendText("TEXT UDP 1.1\n", 0);
    }

    // TCP
    if (pSocket.GetAddressData()->ai_socktype == SOCK_STREAM)
    {
        response = pSocket.ReceiveText(0);
        ss = std::stringstream(response);

        while (getline(ss, temp, '\n'))
        {
            tokens.push_back(temp);
        }

        for (std::string& token : tokens)
        {
            if (token == "TEXT TCP 1.1")
            {
                break;
            }

            if (&token == &tokens.back())
            {
                std::cout << "ERROR: MISSMATCH PROTOCOL\n";
                exit(EXIT_FAILURE);
            };
        }

        pSocket.SendText("TEXT TCP 1.1 OK\n", 0);
    }

    response = pSocket.ReceiveText(0);

    tokens.clear();
    ss = std::stringstream(response);
    while (getline(ss, temp, ' '))
    {
        tokens.push_back(temp);
    }

    if (tokens.size() != 3)
    {
        std::cout << "ERROR: Incorrect data received\n";
        exit(EXIT_FAILURE);
    }

    int operand = 0;
    if (tokens.at(0) == "add")
    {
        operand = 1;
    }
    if (tokens.at(0) == "sub")
    {
        operand = 2;
    }
    if (tokens.at(0) == "mul")
    {
        operand = 3;
    }
    if (tokens.at(0) == "div")
    {
        operand = 4;
    }

    int result = PerformArithmetic(operand, std::stoi(tokens.at(1)), std::stoi(tokens.at(2)));

    pSocket.SendText(std::to_string(result) + "\n", 0);
    response = pSocket.ReceiveText(0);
    if (response != "OK\n")
    {
        std::cout << "ERROR: Server sent error\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "OK (myresult=" << result << ")\n";
}

