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
        std::cout << "ADD: " << pVal1 << " + " << pVal2 << " = " << result << "\n";
        break;

        case 2:
            result = pVal1 - pVal2;
        std::cout << "SUB: " << pVal1 << " - " << pVal2 << " = " << result << "\n";
        break;

        case 3:
            result = pVal1 * pVal2;
        std::cout << "MUL: " << pVal1 << " * " << pVal2 << " = " << result << "\n";
        break;

        case 4:
            result = pVal1 / pVal2;
        std::cout << "DIV: " << pVal1 << " / " << pVal2 << " = " << result << "\n";
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

    pSocket.SendBinary(firstMessage, 0);
    std::variant<calcMessage, calcProtocol> responseVariant = pSocket.ReceiveBinary(0);
    calcProtocol response = std::get<calcProtocol>(responseVariant);

    response.inResult = PerformArithmetic(response.arith, response.inValue1, response.inValue2);

    response.type = 2;
    pSocket.SendBinary(response, 0);
    responseVariant = pSocket.ReceiveBinary(0);
    calcMessage test2 = std::get<calcMessage>(responseVariant);
}

void Misc::PerformTextCommunication(const Socket& pSocket)
{
    std::string response = pSocket.ReceiveText(0);
    pSocket.SendText("TEXT TCP 1.1 OK\n", 0);
    response = pSocket.ReceiveText(0);

    std::vector<std::string> tokens;
    std::string temp;
    std::stringstream ss(response);

    while (getline(ss, temp, ' '))
    {
        tokens.push_back(temp);
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
}

