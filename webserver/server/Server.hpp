#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../parsing/parser.hpp"
#include "../../request_response/Response.hpp"
#include <csignal>
#include <sys/event.h>
#include <limits.h>
#include <iterator>
#include <cstring>
#include <sys/poll.h>
#include <exception>
#include <fstream>
#include "../serverSocket/ServerSocket.hpp"

#define greenColor "\033[32m"
#define redColor "\033[31m"
#define yellowColor "\033[33m"

#define CHUNK_SIZE 40400

#define INIT_EVENT(ev, clinetSocket) \
        ev.fd = clinetSocket;         \
        ev.events = POLLIN | POLLOUT;  \
        ev.revents = ZERO;              \

#define SOCKETFAILED "Error binding socket!"

class Server : virtual public Parser , public Response, public ServerSocket, public Client {
    private:
        pollfd events;
        std::map<int, Client> clientSocket;
        std::vector<ServerSocket> virtualServer;
        std::vector<int> serverPort;
        std::vector<pollfd> tmpEvents;
        std::map<std::string, std::vector<std::pair<std::string, std::string > > > serverUnitsData;
    public:
        virtual ~Server( void );
        Server( void );
        Server( const Server & other );
        Server & operator=(const Server & other);
        void runServer(Parser &p);
        void initializeSocket(  std::vector<server_data> serverData );
        void handleHttpRequest(int clientSocket, Parser &p, int flag, int type);
        bool receiveData(int cSock, int index);
        bool acceptNewConnection( ServerSocket server );
        void POST(int index, Parser &p, int type);
        void GET( int index, Parser &p, int type);
        void DELETE( int index, Parser &p, int type);
        bool Sent(int cSock, int index);
        bool isServer( int cSock );
        int getClientPort(int index);
        void collectServersData(std::vector<server_data> serverData);
};
