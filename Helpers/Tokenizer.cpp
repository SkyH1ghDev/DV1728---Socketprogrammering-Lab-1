#include "Tokenizer.hpp"

namespace Helper
{

    // PUBLIC
    std::array<std::string, 4> Tokenizer::Tokenize(const std::string& pInput)
    {
        return
        {
            GetProtocol(pInput),
            GetHostname(pInput),
            GetPort(pInput),
            GetAPI(pInput)
        };
    }

    // PRIVATE
    std::string Tokenizer::GetProtocol(const std::string& pInput)
    {

    }

    std::string Tokenizer::GetHostname(const std::string& pInput)
    {

    }

    std::string Tokenizer::GetPort(const std::string& pInput)
    {

    }

    std::string Tokenizer::GetAPI(const std::string& pInput)
    {

    }
}