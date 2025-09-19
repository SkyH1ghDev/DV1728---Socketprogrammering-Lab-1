#pragma once
#include "AddrInfo.hpp"
#include "Socket.hpp"

namespace Helper
{
    class Misc
    {
    public:
        static void CheckPortValidity(int pPort);
        static Socket CreateSocket(const AddrInfo& pServerInformation);
        static int PerformArithmetic(int pOperand, int pVal1, int pVal2);

        static void PerformBinaryCommunication(const Socket& pSocket);
        static void PerformTextCommunication(const Socket& pSocket);
    };
}
