#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <protocol.h>
#include <Helpers/Socket.hpp>
#include <Helpers/AddrInfo.hpp>
#include <Helpers/Tokenizer.hpp>

/* You will to add includes here */

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG


// Included to get the support library

int main(const int argc, char* argv[])
{
    // Check whether an argument has been passed
    if (argc < 2)
    {
        std::stringstream ss("Usage: %s protocol://server:port/path. ");
        ss << argv[0] << "\n";
        throw std::runtime_error(ss.str());
    }

    // Getting the argument
    const std::string input = argv[1];

    // Checking whether a triple slash has been
    if (input.find("///") != std::string::npos)
    {
        std::stringstream ss("Invalid format: ");
        ss << input << "\n";
        throw std::runtime_error(ss.str());
    }

    // Tokenize Input
    Helper::Tokenizer tokenizer;
    Helper::TokenizerData ipData = tokenizer.Tokenize(input);

    std::cout << "Host " << ipData.Hostname << ", and port " << ipData.Port << "\n";

    // Check whether port is valid
    if (int port = std::stoi(ipData.Port); port < 1 or port > 65535)
    {
        std::cout << "Error: Port is out of server scope.\n";
        if (port > 65535)
        {
            std::cout << "Error: Port is not a valid UDP or TCP port.\n";
        }
        return 1;
    }

    // Setup socket hints
    addrinfo socketHints{};
    socketHints.ai_family = AF_UNSPEC;

    if (ipData.Protocol == "UDP" || ipData.Protocol == "udp")
    {
        socketHints.ai_socktype = SOCK_DGRAM;
    }

    if (ipData.Protocol == "TCP" || ipData.Protocol == "tcp")
    {
        socketHints.ai_socktype = SOCK_STREAM;
    }

    // Establish server information
    Helper::AddrInfo serverInformation(ipData.Hostname, ipData.Port, socketHints);

    Helper::Socket socket;
    addrinfo* currentAddress = nullptr;
    for (addrinfo* addrInfo = serverInformation.GetAddrInfo(); addrInfo != nullptr; addrInfo = addrInfo->ai_next)
    {
        try
        {
            Helper::Socket testSocket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
            socket = std::move(testSocket);
            currentAddress = std::move(addrInfo);
        } catch (std::exception)
        {
            continue;
        }

        break;
    }

    if (currentAddress == nullptr)
    {
        throw std::runtime_error("Error: Failed to create socket");
    }

    calcMessage calcMsg{};
    calcMsg.type = 22;
    calcMsg.message = 0;
    calcMsg.protocol = 17;
    calcMsg.major_version = 1;
    calcMsg.minor_version = 1;

    socket.SendToBinary(calcMsg, 0, currentAddress);
    std::variant<calcMessage, calcProtocol> response = socket.ReceiveFromBinary(0, currentAddress);
    calcProtocol test = std::get<calcProtocol>(response);

    switch (test.arith)
    {
        case 1:
            test.inResult = test.inValue1 + test.inValue2;
            std::cout << "ADD: " << test.inValue1 << " + " << test.inValue2 << " = " << test.inResult << "\n";
            break;

        case 2:
            test.inResult = test.inValue1 - test.inValue2;
            std::cout << "SUB: " << test.inValue1 << " - " << test.inValue2 << " = " << test.inResult << "\n";
            break;

        case 3:
            test.inResult = test.inValue1 * test.inValue2;
            std::cout << "MUL: " << test.inValue1 << " * " << test.inValue2 << " = " << test.inResult << "\n";
            break;

        case 4:
            test.inResult = test.inValue1 / test.inValue2;
            std::cout << "DIV: " << test.inValue1 << " / " << test.inValue2 << " = " << test.inResult << "\n";
            break;

        default:
            throw std::runtime_error("Error: Undefined arithmetic operator-value received\n");
    }

    test.type = 2;

    socket.SendToBinary(test, 0, currentAddress);
    response = socket.ReceiveFromBinary(0, currentAddress);
    calcMessage test2 = std::get<calcMessage>(response);

#ifdef DEBUG
    std::cout
            << "Protocol: " << ipData.Protocol
            << " Host: " << ipData.Hostname
            << " Port: " << ipData.Port
            << " Path: " << ipData.Path;
#endif

    return 0;
}
