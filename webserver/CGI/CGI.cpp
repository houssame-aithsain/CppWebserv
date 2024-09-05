#include "CGI.hpp"

CGI::~CGI( void ) {

    env.clear();
}

CGI::CGI( void ) {
    
    ERRCODE = 0;
}

CGI::CGI( const CGI & other ) {

    *this = other;
}

CGI & CGI::operator=(const CGI & other) {
    
    if (this != &other) {
        output = other.output;
        request = other.request;
        env = other.env;
        REQUEST_METHOD = other.REQUEST_METHOD;
        CONTENT_LENGTH = other.CONTENT_LENGTH;
        CONTENT_TYPE = other.CONTENT_TYPE;
        PATH_INFO = other.PATH_INFO;
        QUERY_STRING = other.QUERY_STRING;
        PATH = other.PATH;
        ERRCODE = other.ERRCODE;
    }
    return (*this);
}

void CGI::executeCGIscript(std::map<std::string, std::string> data) {

    int status;
    char *const args[] = {const_cast<char *>(PATH.c_str()), nullptr};
    std::ofstream parentTmpFile("/tmp/parent", std::ios::out | std::ios::trunc);
    std::ifstream childTmpFile("/tmp/child", std::ios::out | std::ios::trunc);
    
    parentTmpFile << data["body"];
    parentTmpFile.close();
    
    int childFD = open("/tmp/child", O_WRONLY);
    int parentFD = open("/tmp/parent", O_RDONLY);

    if (parentFD < 0) {
        ERRCODE = 500;
        return;
    }

    int pid = fork();
    if (pid == 0) {
        dup2(parentFD, STDIN_FILENO);
        dup2(childFD, STDOUT_FILENO);
        childTmpFile.close();
        parentTmpFile.close();
        close(parentFD);
        close(childFD);
        execve(args[0], args, env.data());
    } else if (pid > 0) {
        std::string ll;
        close(parentFD);
        close(childFD);
        clock_t start = clock();
        while (!waitpid(pid, &status, WNOHANG)) {
            if (((clock() - start) / CLOCKS_PER_SEC) > 5) {
                kill(pid, SIGKILL);
                ERRCODE = 504;
                std::cout << "CGI: time out try agian!" << std::endl;
                break;
            }
        }
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status)) {
                ERRCODE = 500;
            }
        }
        output.clear();
        std::ifstream readChildTmpFile("/tmp/child");
        if (readChildTmpFile.is_open()) {
            while (std::getline(readChildTmpFile, ll)) {
                output += ll + "\r\n";
            }
            readChildTmpFile.close();
        } else {
            std::cerr << "Failed to open childTmpFile for reading." << std::endl;
        }
    } else {
        perror("fork");
        close(parentFD);
        close(childFD);
    }
    childTmpFile.close();
}

void CGI::GETenvInit(std::map<std::string, std::string> data) {

    REQUEST_METHOD = "REQUEST_METHOD=GET";
    CONTENT_TYPE = "CONTENT_TYPE=" + data["Content-Type"];
    QUERY_STRING = "QUERY_STRING=" + data["QUERY_STRING"];
    CONTENT_TYPE.erase(CONTENT_TYPE.end() - 1);
    env.push_back((char *)REQUEST_METHOD.c_str());
    env.push_back((char *)QUERY_STRING.c_str());
    env.push_back((char *)CONTENT_TYPE.c_str());
    env.push_back(nullptr);
}

void CGI::POSTenvInit(Client client, std::map<std::string, std::string> data) {

    std::string lent;
    std::stringstream ss;

    ss << client.getResponseBodyCounter();
    lent = ss.str();
    REQUEST_METHOD = "REQUEST_METHOD=POST";
    CONTENT_LENGTH = "CONTENT_LENGTH=" + lent;
    CONTENT_TYPE = "CONTENT_TYPE=" + data["Content-Type"];
    CONTENT_TYPE.erase(CONTENT_TYPE.end() - 1);
    env.push_back((char *)REQUEST_METHOD.c_str());
    env.push_back((char *)CONTENT_LENGTH.c_str());
    env.push_back((char *)CONTENT_TYPE.c_str());
    env.push_back(nullptr);
}

std::string CGI::extractClientContent(Client & client, std::map<std::string, std::string> data) 
{
    ERRCODE = 0;
    data["body"] = client.getPostBuffer().substr(0, client.getPostBuffer().find("\r\n\r\n")) + "\r\n\r\n" +  data["body"];
    PATH =  data["PWD"];
    std::cout << "path: " << PATH << std::endl;
    if (data["Method"] == "GET") {
        GETenvInit(data);
    }
    else if (data["Method"] == "POST") {
        POSTenvInit(client, data);
    }
    executeCGIscript(data);
    env.clear();
    return output;
}
