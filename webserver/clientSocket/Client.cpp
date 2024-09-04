#include "Client.hpp"

Client::~Client( void ) {}

Client::Client( void ) : remainingBytes(0), totalBytesSent(0), responseBodyCounter(0) {}

Client::Client( pollfd ev) : event(ev), remainingBytes(0), totalBytesSent(0), responseBodyCounter(0) {}

Client::Client( const Client & other ) {
        
    *this = other;
}

Client & Client::operator=(const Client & other) {
    
    if (this != &other) {
        event = other.event;
        requestBuffer = other.requestBuffer;
        responseBuffer = other.responseBuffer;
        remainingBytes = other.remainingBytes;
        totalBytesSent = other.totalBytesSent;
        responseBodyCounter = other.responseBodyCounter;
    }
    return (*this);
}

void Client::clearBuffer( void ) {requestBuffer.clear();}

pollfd& Client::getPollFd( void ) {return (event);}

int Client::getFD( void ) {return (event.fd);}

std::string Client::getBuffer( void ) {return (requestBuffer);}

void Client::appendStr(char *chunk, size_t bytesSize) {requestBuffer.append(chunk, bytesSize);}

void Client::changePollFd(pollfd me) {event = me;}

void Client::fillResponseBuffer(std::string response) {responseBuffer = response;}

std::string Client::getResponseBuffer( void ) {return (responseBuffer);}

void Client::clearResponseBuffer( void ) {responseBuffer.clear();}

void Client::setRemainingBytes( size_t bytes ) {remainingBytes = bytes;}

void Client::unappendRemainingBytes( size_t bytes ) {remainingBytes -= bytes;}

void Client::resetRemainingBytes( void ) {remainingBytes = 0;}

size_t Client::getRemainingBytes( void ) {return (remainingBytes);}

void Client::appendTotalBytesSent( size_t bytes ) {totalBytesSent += bytes;}

void Client::resetTotalBytesSent( void ) {totalBytesSent = 0;}

size_t Client::getTotalBytesSent( void ) {return (totalBytesSent);}

void Client::setPollfd(const pollfd pfd) {event = pfd;}

std::string Client::getPostBuffer( void ) {return (Postbuffer);}

void Client::appendPostBuffer( std::string pb ) {Postbuffer += pb;}

void Client::clearPostBuffer( void ) {Postbuffer.clear();}

void Client::initResponseBodyCounter( size_t count ) {responseBodyCounter = count;}

void Client::resetResponseBodyCounter( void ) {responseBodyCounter = 0;}

size_t Client::getResponseBodyCounter( void ) {return (responseBodyCounter);}

void Client::clear( void ) {

    close(event.fd);
    requestBuffer.clear();
    responseBuffer.clear();
    Postbuffer.clear();
    remainingBytes = 0;
    totalBytesSent = 0;
    responseBodyCounter = 0;
}
