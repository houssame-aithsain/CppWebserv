#include "Server.hpp"
#include <sstream>
#include <map>

Server::~Server( void ) {}

Server::Server( void ) {}

Server::Server( const Server & other ) {

    *this = other;
}

Server & Server::operator=(const Server & other) {

    if (this != &other) {
        this->clientSocket = other.clientSocket;
        this->virtualServer = other.virtualServer;
        this->serverPort = other.serverPort;
        this->tmpEvents = other.tmpEvents;
        this->serverUnitsData = other.serverUnitsData;
    }
    return (*this);
}

int Server::getClientPort(int index) {
    
    int cS = tmpEvents[index].fd;
    sockaddr_in clientAddress;
    socklen_t addrLen = sizeof(clientAddress);
    int clientPort = 0x0000;

    if (getsockname(cS, reinterpret_cast<sockaddr*>(&clientAddress), &addrLen) == 0) {
        clientPort = ntohs(clientAddress.sin_port);
    }
    return clientPort;
}

bool Server::isServer( int cSock ) {

    for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) {
        if (it->getServerSocketFd() == cSock)
            return true;
    }
    return false;
}

void Server::collectServersData(std::vector<server_data> serverData) {

    std::string host;
    std::string server_name;

    for (std::vector<server_data>::iterator it = serverData.begin(); it != serverData.end(); it++) {
        for (size_t x = 0; x < it->server.size(); x++) {
            if (it->server[x].first == "host")
                host = it->server[x].second[0];
            if (it->server[x].first == "server_name")
                server_name = it->server[x].second[0];
        }
        for (size_t x = 0; x < it->server.size(); x++) {
            for (size_t i = 0; i < it->server[x].second.size() && it->server[x].first == "port"; i++) {
                serverUnitsData[it->server[x].second[i]].push_back(std::make_pair(host, server_name));
            }
        }
    }
}

void Server::initializeSocket(std::vector<server_data> serverData) {
    
    std::vector<std::string> bindedHosts;
    std::vector<std::string> bindedPorts;
    signal(SIGPIPE, SIG_IGN);
    collectServersData(serverData);

    for (std::map<std::string, std::vector<std::pair<std::string, std::string > > >::iterator it = serverUnitsData.begin(); it != serverUnitsData.end(); it++) {
        bindedPorts.push_back(it->first);
        for (size_t i = ZERO; i < it->second.size(); i++) {
            if ((std::find(bindedHosts.begin(), bindedHosts.end(), it->second[i].first) == bindedHosts.end() || std::find(bindedPorts.begin(), bindedPorts.end(), it->first) == bindedPorts.end()) || !i) {
                virtualServer.push_back(ServerSocket(atoi(it->first.c_str()), it->second[i].first, it->second[i].second));
                INIT_EVENT(events, virtualServer.back().getServerSocketFd());
                tmpEvents.push_back(events);
                bindedHosts.push_back(it->second[i].first);
            }
        }
    }
}

bool Server::Sent(int cSock, int index) {

    ssize_t bytesSent = send(cSock, clientSocket[cSock].getResponseBuffer().c_str() + clientSocket[cSock].getTotalBytesSent(), clientSocket[cSock].getRemainingBytes(), 0);
    if (bytesSent == FAILED || !bytesSent) { 
        if (bytesSent == FAILED) {
            return false;
        }
        close(cSock);
        clientSocket.erase(cSock);
        tmpEvents.erase(tmpEvents.begin() + index);
        return false;
    }
    clientSocket[cSock].appendTotalBytesSent(bytesSent);
    clientSocket[cSock].unappendRemainingBytes(bytesSent);
    if (!clientSocket[cSock].getRemainingBytes()) {
        close(cSock);
        clientSocket[cSock].resetRemainingBytes();
        clientSocket[cSock].resetTotalBytesSent();
        clientSocket[cSock].clearResponseBuffer();
    }
    std::cout << greenColor << "Sending...." << resetColor << std::endl;
    return true;
}

bool Server::receiveData(int cSock, int index) {

    char chunk[CHUNK_SIZE + 1];

    bzero(chunk, CHUNK_SIZE + 1);
    int bytesRead = recv(cSock, chunk, CHUNK_SIZE, ZERO);
    if (bytesRead == FAILED) {
            return false;
    } else if (!bytesRead) {
        close(cSock);
        clientSocket.erase(cSock);
        tmpEvents.erase(tmpEvents.begin() + index);
        return false;
    }
    chunk[bytesRead] = ZERO;
    clientSocket[cSock].appendStr(chunk, bytesRead);
    std::cout << yellowColor << "Receiving...." << resetColor << std::endl;
    return true;
}
