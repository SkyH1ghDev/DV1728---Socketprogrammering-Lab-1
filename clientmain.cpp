#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <Helpers/Tokenizer.hpp>

/* You will to add includes here */

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG


// Included to get the support library

int main(const int argc, char *argv[])
{
    const std::size_t hostLength = 2000;
    const std::size_t portLength = 6;
    const std::size_t pathLength = 7;

    if (argc < 2)
    {
        std::cerr << "Usage: %s protocol://server:port/path." << argv[0];
        exit(EXIT_FAILURE);
    }
  
    /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port).
    Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'.
    */
    /* protocolString size = 6
     * hostString size = 2000
     * portString size = 6
     * pathString size = 7
     */
    std::string hostString, portString, pathString;
    const std::string input = argv[1];

    (void)Helper::Tokenizer::Tokenize(input);
    
    /* Some error checks on string before processing */
    // Check for more than two consecutive slashes '///'.
    if (input.find("///") != std::string::npos){
        std::cerr << "Invalid format: " << input << "\n";
        return 1;
    }


    // Find the position of "://"
    const std::size_t protocolEnd = input.find("://");
    if (protocolEnd == std::string::npos)
    {
        std::cerr << "Invalid format: missing '://'\n";
        return 1;
    }

    // Extract protocol
    const std::string protocolString = input.substr(0, protocolEnd);
    if (constexpr std::size_t maxProtLen = 6; protocolString.size() >= maxProtLen)
    {
        std::cerr << "Error: Protocol string too long\n";
        return 1;
    }
    /*
    // Move past "://"
    char *host_start = proto_end + 3;

    // Find the position of ":"
    char *port_start = strchr(host_start, ':');
    if (!port_start || port_start == host_start) {
	printf("Error: Port is missing or ':' is misplaced\n");
        return 1;
    }

    // Extract host
    size_t host_len = port_start - host_start;
    if (host_len >= sizeof(hostString)) {
        printf("Error: Host string too long\n");
        return 1;
    }
    
    // Copy host
    strncpy(hostString, host_start, port_start - host_start);
    hostString[port_start - host_start] = '\0';

        // Find '/' which starts the path
    char *path_start = strchr(host_start, '/');
    if (!path_start || *(path_start + 1) == '\0') {
        fprintf(stderr, "Error: Path is missing or invalid\n");
        return 1;
    }

    // Extract path
    if (strlen(path_start + 1) >= sizeof(pathString)) {
        fprintf(stderr, "Error: Path string too long\n");
        return 1;
    }
    strcpy(pathString, path_start + 1);

    // Extract port


    size_t port_len = path_start - port_start - 1;
    if (port_len >= sizeof(portString)) {
        fprintf(stderr, "Error: Port string too long\n");
        return 1;
    }
    strncpy(portString, port_start + 1, port_len);
    portString[port_len] = '\0';

    // Validate port is numeric
    for (size_t i = 0; i < strlen(portString); ++i) {
        if (portString[i] < '0' || portString[i] > '9') {
            fprintf(stderr, "Error: Port must be numeric\n");
            return 1;
        }
    }


    
    char *protocol, *Desthost, *Destport, *Destpath;
    protocol = protocolString;
    Desthost = hostString;
    Destport = portString;
    Destpath = pathString;
      
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 


    
  /* Do magic
  int port=atoi(Destport);
  if (port < 1000 or port >65535) {
    printf("Error: Port is out of server scope.\n");
    if ( port > 65535 ) {
      printf("Error: Port is not a valid UDP or TCP port.\n");
    }
    return 1;
  }
#ifdef DEBUG 
  printf("Protocol: %s Host %s, port = %d and path = %s.\n",protocol, Desthost,port, Destpath);
#endif*/


}
