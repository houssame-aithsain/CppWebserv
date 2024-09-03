#include "ServerSocket.hpp"
#include <arpa/inet.h>

ServerSocket::~ServerSocket( void ) {}

ServerSocket::ServerSocket( void ) {}

ServerSocket::ServerSocket( const ServerSocket & other) {

    *this = other;
}

ServerSocket & ServerSocket::operator=(const ServerSocket & other) {

    if (this != &other) {
        this->sockPort = other.sockPort;
        this->hostName = other.hostName;
        this->serverName = other.serverName;
        this->socketAddress = other.socketAddress;
        this->serverSocketFd = other.serverSocketFd;
    }
    return (*this);
}

ServerSocket::ServerSocket( int port, std::string host, std::string name ) : sockPort(port), hostName( host ), serverName( name ) {
    sockCreat();
    INIT_SOCKET(socketAddress, sockPort);
    reuseSocket();
    nonBlockingMode();
    sockBind();
    sockListen();
}

void ServerSocket::sockCreat(void) {serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);}

int ServerSocket::getServerSocketFd( void ) {return (serverSocketFd);}

void ServerSocket::reuseSocket( void ) {

    int reuse = ONE;
    if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        throw "setsockopt(SO_REUSEADDR) failed";
    }
}

void ServerSocket::nonBlockingMode( void ) {

    if (fcntl(serverSocketFd, F_SETFL, O_NONBLOCK) == FAILED) {
        throw "calling fcntl";
    }
}

void ServerSocket::sockBind( void ) {

    if (bind(serverSocketFd, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        close(serverSocketFd);
        throw "failed to bind server socket";
    }
}

void ServerSocket::sockListen( void ) {

    if (listen(serverSocketFd, 1000) == FAILED) {
        close(serverSocketFd);
        throw "failed to make the socket at the listen mode!";
    }
    std::cout << blueColor << "listening on port "<< resetColor << sockPort << " | ip: " << hostName << " | SVname: " << serverName << std::endl;
}

sockaddr_in& ServerSocket::getSocketAddress( void ) {return (socketAddress);}

int ServerSocket::getServerPort( void ) {return (ntohs(socketAddress.sin_port));}