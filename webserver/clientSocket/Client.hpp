#pragma once

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/poll.h>

class Client {
    private:
        pollfd event;
        std::string requestBuffer;
        std::string responseBuffer;
        std::string Postbuffer;
        int remainingBytes;
        int totalBytesSent;
        size_t responseBodyCounter;
    public:
        ~Client( void );
        Client( void );
        Client( const Client & other );
        Client & operator=(const Client & other);
        Client( pollfd ev);
        void clearBuffer( void );
        pollfd& getPollFd( void );
        void setPollfd(const pollfd pfd);
        int getFD( void );
        std::string getBuffer( void );
        void appendStr(char *chunk, size_t bytesSize) ;
        void changePollFd(pollfd me);
        void fillResponseBuffer(std::string response);
        std::string getResponseBuffer( void );
        void clearResponseBuffer( void );
        void setRemainingBytes( size_t bytes );
        void unappendRemainingBytes( size_t bytes );
        void resetRemainingBytes( void );
        size_t getRemainingBytes( void );
        void appendTotalBytesSent( size_t bytes );
        void resetTotalBytesSent( void );
        size_t getTotalBytesSent( void );
        std::string getPostBuffer( void );
        void appendPostBuffer( std::string );
        void clearPostBuffer( void );
        void initResponseBodyCounter( size_t count);
        void resetResponseBodyCounter( void );
        size_t getResponseBodyCounter( void );
        void clear( void );
        void erase( void );
};
