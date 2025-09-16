#pragma once

#include <string>

namespace Helper
{
    struct TokenizerData
    {
        std::string Protocol = nullptr;
        std::string Hostname = nullptr;
        std::string Port = nullptr;
        std::string Path = nullptr;
    };

    class Tokenizer
    {
    public:
        TokenizerData Tokenize(std::string_view pInput);

    private:
        std::string GetProtocol(std::string_view pInput);

        std::string GetHostname(std::string_view pInput);

        std::string GetPort(std::string_view pInput);

        std::string GetPath(std::string_view pInput);

    private:
        struct TokenizerOptions
        {
            std::size_t maxProtocolLength = 6;
            std::size_t maxHostnameLength = 2000;
            std::size_t maxPortLength = 6;
            std::size_t maxPathLength = 7;
        } m_tokenizerOptions;

        std::size_t m_stringCounter = 0;
    };
} // Helper
