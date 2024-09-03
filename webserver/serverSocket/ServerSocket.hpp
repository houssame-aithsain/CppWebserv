#include <netinet/in.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#define ZERO 0
#define ONE 1
#define FAILED -1

#define blueColor "\033[34m"
#define resetColor "\033[0m"

#define INIT_SOCKET(socketAddress, PORT)     \
    socketAddress.sin_family = AF_INET;       \
    socketAddress.sin_addr.s_addr = inet_addr(hostName.c_str()); \
    socketAddress.sin_port = htons(PORT);       \

class ServerSocket {
    private:
        int sockPort;
        std::string hostName;
        std::string serverName;
        sockaddr_in socketAddress;
        int serverSocketFd;
    public:
        ~ServerSocket( void );
        ServerSocket( void );
        ServerSocket( const ServerSocket & other);
        ServerSocket( int port, std::string host, std::string name);
        ServerSocket & operator=(const ServerSocket & other);
        void sockCreat( void );
        void reuseSocket( void );
        void nonBlockingMode( void );
        void sockBind( void );
        void sockListen( void );
        int getServerSocketFd( void );
        sockaddr_in& getSocketAddress( void );
        int getServerPort( void );
};
