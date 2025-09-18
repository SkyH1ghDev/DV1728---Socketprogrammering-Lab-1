#include <iostream>
#include <string>
#include <sstream>
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
        std::stringstream ss("Usage: %s protocol://server:port/path. "); ss << argv[0] << "\n";
        throw std::runtime_error(ss.str());
    }

    // Getting the argument
    const std::string input = argv[1];

    // Checking whether a triple slash has been
    if (input.find("///") != std::string::npos)
    {
        std::stringstream ss("Invalid format: "); ss << input << "\n";
        throw std::runtime_error(ss.str());
    }

    Helper::Tokenizer tokenizer;
    Helper::TokenizerData ipData = tokenizer.Tokenize(input);

    if (int port = std::stoi(ipData.Port); port < 1000 or port > 65535) {
        std::cout << "Error: Port is out of server scope.\n";
        if ( port > 65535 ) {
            std::cout << "Error: Port is not a valid UDP or TCP port.\n";
        }
        return 1;
    }

    Helper::AddrInfo addressInformation = Helper::AddrInfo(ipData.Hostname, ipData.Port, nullptr);
    Helper::Socket socket;
    for (addrinfo* addrInfo = addressInformation.GetAddrInfo(); addrInfo != nullptr; addrInfo = addrInfo->ai_next)
    {
        try
        {
            Helper::Socket testSocket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
            socket = testSocket;
        }
        catch (std::runtime_error())
        {

        }

        break;
    }


#ifdef DEBUG
    std::cout
        << "Protocol: " << ipData.Protocol
        << " Host: " << ipData.Hostname
        << " Port: " << ipData.Port
        << " Path: " << ipData.Path;
#endif

    return 0;
}
