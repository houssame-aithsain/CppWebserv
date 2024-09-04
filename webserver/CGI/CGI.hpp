#pragma once

#include "../clientSocket/Client.hpp"
#include <string>
#include <sstream>
#include <signal.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <map>

class CGI {
    protected:
        std::string output;
        std::string request;
        std::vector<char *> env;
        std::string REQUEST_METHOD;
        std::string CONTENT_LENGTH;
        std::string CONTENT_TYPE;
        std::string PATH_INFO;
        std::string QUERY_STRING;
        std::string PATH;
        int ERRCODE;
    public:
        virtual ~CGI( void );
        CGI( void );
        CGI( const CGI & other );
        CGI & operator=(const CGI & other);
        std::string extractClientContent(Client & client, std::map<std::string, std::string> data);
        void executeCGIscript(std::map<std::string, std::string> data);
        std::string getContentType(std::map<std::string, std::string> data);
        void GETenvInit(std::map<std::string, std::string> data);
        void POSTenvInit(Client client, std::map<std::string, std::string> data);
};
