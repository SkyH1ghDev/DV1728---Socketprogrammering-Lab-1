#pragma once

#include <array>
#include <string>
#include <cstdlib>

namespace Helper
{

    class Tokenizer
    {
        public:
            static std::array<std::string, 4> Tokenize(const std::string& pInput);

		private:
			static std::string GetProtocol(const std::string& pInput);
			static std::string GetHostname(const std::string& pInput);
			static std::string GetPort(const std::string& pInput);
			static std::string GetAPI(const std::string& pInput);

        private:

        	std::size_t stringCounter = 0;
    };

} // Helper