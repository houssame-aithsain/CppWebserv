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

void Server::handleHttpRequest(int cSock, Parser &p, int flag, int type)
{
    if (flag == 1 && this->_errorCode == 0) {
        createResponse(p, type, clientSocket[cSock]);
    }
    clientSocket[cSock].resetRemainingBytes();
    clientSocket[cSock].resetTotalBytesSent();
    clientSocket[cSock].clearBuffer();
    clientSocket[cSock].clearResponseBuffer();
    clientSocket[cSock].fillResponseBuffer(this->response);
    clientSocket[cSock].setRemainingBytes(this->response.length());
    clientSocket[cSock].clearBuffer();
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

void Server::GET(int index,  Parser &p, int type) {

    std::cout << blueColor << "GET" << resetColor << std::endl;
    this->_errorCode = 0;
    std::string pr = this->getPort();
    int check_error = requestErrorsHandling(p, pr);
    if (this->_errorCode || check_error == 1) {
        handleHttpRequest(tmpEvents[index].fd, p, 0, 0);
    }
    else if (this->request_data["Method"] == "GET" && check_error == 0) {
        handleHttpRequest(tmpEvents[index].fd, p, 1, type);
    }
}

void Server::POST(int index, Parser &p, int type) {

    std::cout << blueColor << "POST" << resetColor << std::endl;
    clientSocket[tmpEvents[index].fd].appendPostBuffer(clientSocket[tmpEvents[index].fd].getBuffer());
    if (clientSocket[tmpEvents[index].fd].getResponseBodyCounter() <= clientSocket[tmpEvents[index].fd].getPostBuffer().size()) {
        ft_parse_request(clientSocket[tmpEvents[index].fd].getPostBuffer());
        this->_errorCode = 0;
        std::string pr = this->getPort();
        int check = requestErrorsHandling(p, pr);
        if (check == 0) {
            handleHttpRequest(tmpEvents[index].fd, p, 1, type);
        }
        clientSocket[tmpEvents[index].fd].fillResponseBuffer(this->response);
        clientSocket[tmpEvents[index].fd].setRemainingBytes(this->response.length());
        Sent(tmpEvents[index].fd, index);
        close(tmpEvents[index].fd);
        clientSocket[tmpEvents[index].fd].clearPostBuffer();
        clientSocket[tmpEvents[index].fd].resetResponseBodyCounter();
    }
    clientSocket[tmpEvents[index].fd].clearBuffer();
}

void Server::DELETE(int index,  Parser &p, int type) {

    (void) type;
    std::cout << blueColor << "DELETE" << resetColor << std::endl;
    handleHttpRequest(tmpEvents[index].fd, p, 1, 2);
    clientSocket[tmpEvents[index].fd].clearPostBuffer();
    clientSocket[tmpEvents[index].fd].resetResponseBodyCounter();
    this->_errorCode = 0;
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

bool Server::acceptNewConnection( ServerSocket server ) {

    int cSock;
    pollfd newClient;
    int addrlen = sizeof(server.getSocketAddress());
    
    if ((cSock = accept(server.getServerSocketFd(), (struct sockaddr *)(&server.getSocketAddress()), (socklen_t *)&addrlen)) == FAILED) {
        return false;
    } else {
        int status = fcntl(cSock, F_SETFL, O_NONBLOCK);
        if (status == FAILED) {
            perror("calling fcntl");
        }
        INIT_EVENT(newClient, cSock);
        tmpEvents.push_back(newClient);
        clientSocket.insert(std::make_pair(newClient.fd, Client(newClient)));
    }
    return true;
}

void Server::runServer(Parser &p) {

    while (true) {
        poll(tmpEvents.data(), tmpEvents.size(), -1);
        for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) {
            for (size_t i = 0; i < tmpEvents.size(); i++) {
                if (getClientPort(i) != it->getServerPort() && i)
                    continue;
                if (tmpEvents[i].fd == it->getServerSocketFd()) {
                    if (!this->acceptNewConnection(*it)) {
                        continue;
                    }
                }
                if (tmpEvents[i].revents & POLLIN && !isServer(tmpEvents[i].fd)) {
                    if (!receiveData(tmpEvents[i].fd, i)) {
                        break;
                    }
                    if (!clientSocket[tmpEvents[i].fd].getRemainingBytes() && !clientSocket[tmpEvents[i].fd].getResponseBodyCounter())
                    {
                        ft_parse_request(clientSocket[tmpEvents[i].fd].getBuffer());
                        if (request_data["Method"] != "GET" && request_data["Method"] != "POST" && request_data["Method"] != "DELETE") {
                            close(tmpEvents[i].fd);
                            clientSocket.erase(tmpEvents[i].fd);
                            tmpEvents.erase(tmpEvents.begin() + i);
                            clientSocket[tmpEvents[i].fd].clearBuffer();
                            request_data.clear();
                            continue;
                        }
                    }
                    if (request_data["Method"] == "GET")
                    {
                        GET(i, p, 1);
                    }
                    else if (request_data["Method"] == "DELETE") {
                        DELETE(i, p, 0);
                    }
                    else {
                        if (clientSocket[tmpEvents[i].fd].getPostBuffer().empty()) {
                            std::map<std::string, std::string>::iterator it = request_data.find("Content-Length");
                            clientSocket[tmpEvents[i].fd].initResponseBodyCounter((size_t) atoi(it->second.c_str()) + size_header);
                        }
                        POST(i, p, 0);
                    }
                }
                if (tmpEvents[i].revents & POLLOUT && !isServer(tmpEvents[i].fd)) {
                    if (clientSocket[tmpEvents[i].fd].getRemainingBytes()) {
                        if (!Sent(tmpEvents[i].fd, i))
                            break;
                    }
                }
                clientSocket[tmpEvents[i].fd].clearBuffer();
                request_data.clear();
            }
        }
    }
}
