#include "Tokenizer.hpp"
#include <iostream>

using namespace Helper;

// PUBLIC
TokenizerData Tokenizer::Tokenize(std::string_view pInput)
{
    m_stringCounter = 0;

    return
    {
        GetProtocol(pInput),
        GetHostname(pInput),
        GetPort(pInput),
        GetPath(pInput)
    };
}

// PRIVATE
std::string Tokenizer::GetProtocol(std::string_view pInput)
{
    // Find the position of "://"
    const std::size_t protocolEnd = pInput.find("://");
    if (protocolEnd == std::string::npos)
    {
        throw std::runtime_error("Invalid format: missing '://'\n");
    }

    // Extract protocol
    const std::string_view protocolStringView = pInput.substr(0, protocolEnd);
    if (protocolStringView.size() >= m_tokenizerOptions.maxProtocolLength)
    {
        throw std::runtime_error("Error: Protocol string too long\n");
    }

    m_stringCounter += protocolStringView.size() + 3; // Skipping '://'

    return {protocolStringView.begin(), protocolStringView.end()};
}

std::string Tokenizer::GetHostname(std::string_view pInput)
{
    const std::size_t hostEnd = pInput.find(':', m_stringCounter);
    if (hostEnd == std::string::npos || hostEnd == m_stringCounter)
    {
        throw std::runtime_error("Error: Port is missing or ':' is misplaced");
    }

    const std::string_view hostStringView = pInput.substr(m_stringCounter, hostEnd - m_stringCounter);
    if (hostStringView.size() >= m_tokenizerOptions.maxHostnameLength)
    {
        throw std::runtime_error("Error: Host string too long");
    }

    m_stringCounter += hostStringView.size() + 1; // Skipping ':'

    return {hostStringView.begin(), hostStringView.end()};
}

std::string Tokenizer::GetPort(std::string_view pInput)
{
    const std::size_t portEnd = pInput.find('/', m_stringCounter);
    if (portEnd == std::string::npos || portEnd == m_stringCounter)
    {
        throw std::runtime_error("Error: Path is missing or invalid");
    }

    const std::string_view portStringView = pInput.substr(m_stringCounter, portEnd - m_stringCounter);
    if (portStringView.size() >= m_tokenizerOptions.maxPortLength)
    {
        throw std::runtime_error("Error: Port string too long");
    }

    for (const auto& character : portStringView)
    {
        if (character < '0' || character > '9')
        {
            throw std::runtime_error("Error: Port must be numeric");
        }
    }

    m_stringCounter += portStringView.size() + 1; // Skipping '/'

    return {portStringView.begin(), portStringView.end()};
}

std::string Tokenizer::GetPath(std::string_view pInput)
{
    const std::string_view pathStringView = pInput.substr(m_stringCounter);
    if (pathStringView.size() >= m_tokenizerOptions.maxPathLength)
    {
        throw std::runtime_error("Error: Path string too long");
    }

    return {pathStringView.begin(), pathStringView.end()};
}
