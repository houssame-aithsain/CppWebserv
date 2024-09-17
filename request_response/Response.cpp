#include "Response.hpp"

Response::Response() 
{
    initMapErrors();
    this->_errorCode = 0;
}

Response::~Response()
{
}


std::string to_string(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// getResourceType() is a method that gets the resource type. It takes a string as an argument and returns a string.
std::string getResourceType(const std::string& path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    if (S_ISREG(path_stat.st_mode)) 
        return "file";
    else if (S_ISDIR(path_stat.st_mode)) 
        return "folder";
    else 
        return "other";
}

// msg_code_arr() is a method that initializes the message code array. It takes no arguments and returns nothing.
void Response::msg_code_arr()
{
    msg_code[100] = "Continue";
    msg_code[101] = "Switching Protocols";
    msg_code[200] = "OK";
    msg_code[201] = "Created";
    msg_code[202] = "Accepted";
    msg_code[203] = "Non-Authoritative Information";
    msg_code[204] = "No Content";
    msg_code[205] = "Reset Content";
    msg_code[206] = "Partial Content";
    msg_code[300] = "Multiple Choices";
    msg_code[301] = "Moved Permanently";
    msg_code[302] = "Found";
    msg_code[303] = "See Other";
    msg_code[304] = "Not Modified";
    msg_code[305] = "Use Proxy";
    msg_code[307] = "Temporary Redirect";
    msg_code[400] = "Bad Request";
    msg_code[401] = "Unauthorized";
    msg_code[402] = "Payment Required";
    msg_code[403] = "Forbidden";
    msg_code[404] = "Not Found";
    msg_code[405] = "Method Not Allowed";
    msg_code[406] = "Not Acceptable";
    msg_code[407] = "Proxy Authentication Required";
    msg_code[408] = "Request Timeout";
    msg_code[409] = "Conflict";
    msg_code[410] = "Gone";
    msg_code[411] = "Length Required";
    msg_code[412] = "Precondition Failed";
    msg_code[413] = "Request Entity Too Large";
    msg_code[414] = "Request-URI Too Long";
    msg_code[415] = "Unsupported Media Type";
    msg_code[416] = "Requested Range Not Satisfiable";
    msg_code[417] = "Expectation Failed";
    msg_code[500] = "Internal Server Error";
    msg_code[501] = "Not Implemented";
    msg_code[502] = "Bad Gateway";
    msg_code[503] = "Service Unavailable";
    msg_code[504] = "Gateway Timeout";
    msg_code[505] = "HTTP Version Not Supported";
}

// get_msg() is a method that gets the message. It takes an integer as an argument and returns a string.
std::string Response::get_msg(int code) 
{
    msg_code_arr();
    std::map<int, std::string>::iterator it = msg_code.find(code);
    if (it != msg_code.end()) 
        return it->second;
    return "";
}

// ft_cut_file_from_req() is a method that cuts the file from the request. It takes a string as an argument and returns a string.
std::string ft_cut_file_from_req(std::string &rs)
{
    size_t find_dot = rs.rfind('.');

    if (find_dot == std::string::npos)
        rs += '/';

    size_t lastSlashPos = rs.rfind('/');
    std::string getp = rs.substr(0, lastSlashPos + 1);

    if (lastSlashPos != std::string::npos) 
        return (getp);
    return ("");
}

// ft_getPageToRender() is a method that gets the page to render. It takes a vector of pairs of strings and vectors of strings as an argument and returns a string.
std::string Response::ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest)
{
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator it = ParsedRequest.begin(); it != ParsedRequest.end(); ++it)
    {
        std::vector<std::string> requestDataParsed = it->second;
        if (it->first == "URI")
            return (requestDataParsed.at(0));
    }
    return ("");
}

// initMapErrors() is a method that initializes the error codes map. It takes no arguments and returns nothing.
void Response::initMapErrors()
{
    this->_errorCodes[504] = "HTTP/1.1 504 Gateway Timeout\r\nContent-Length: 54\r\n\r\n<html><body><h1>504 Gateway Timeout</h1></body></html>";
    this->_errorCodes[200] = "HTTP/1.1 200 OK\r\nContent-Length: 46\r\n\r\n<html><body><h1>200 SUCCESS</h1></body></html>";
    this->_errorCodes[400] = "HTTP/1.1 400 Bad Request\r\nContent-Length: 50\r\n\r\n<html><body><h1>400 Bad Request</h1></body></html>";
    this->_errorCodes[401] = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 51\r\n\r\n<html><body><h1>401 Unauthorized</h1></body></html>";
    this->_errorCodes[403] = "HTTP/1.1 403 Forbidden\r\nContent-Length: 48\r\n\r\n<html><body><h1>403 Forbidden</h1></body></html>";
    this->_errorCodes[404] = "HTTP/1.1 404 Not Found\r\nContent-Length: 48\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    this->_errorCodes[405] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 57\r\n\r\n<html><body><h1>405 Method Not Allowed</h1></body></html>";
    this->_errorCodes[409] = "HTTP/1.1 409 Conflict\r\ncontent-type: text/html\r\nContent-Length: 47\r\n\r\n<html><body><h1>409 Conflict</h1></body></html>";
    this->_errorCodes[413] = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 56\r\n\r\n<html><body><h1>413 Payload Too Large</h1></body></html>";
    this->_errorCodes[500] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 60\r\n\r\n<html><body><h1>500 Internal Server Error</h1></body></html>";
    this->_errorCodes[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 54\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
    this->_errorCodes[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: 65\r\n\r\n<html><body><h1>505 HTTP Version Not Supported</h1></body></html>";
    this->_errorCodes[507] = "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 59\r\n\r\n<html><body><h1>507 Insufficient Storage</h1></body></html>";
}

// ft_trim() is a method that trims the string. It takes a string and a string as arguments and returns a string.
std::string ft_trim(const std::string& str, std::string trim)
{
    size_t first = str.find_first_not_of(trim);
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(trim);
    return str.substr(first, (last - first + 1));
}

// ForbiddenChars() is a method that checks for forbidden characters. It takes a string as an argument and returns a boolean.
bool ForbiddenChars(const std::string& str)
{
    const std::string forbiddenChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    
    for (size_t i = 0; str[i]; i++)
        if (forbiddenChars.find(str[i]) == std::string::npos)
            return true;
    return false;
}

// getPort() is a method that gets the port. It takes no arguments and returns a string.
std::string Response::getPort()
{
    size_t pos = this->request_data["Host"].find_last_of(':');
    if (pos == std::string::npos)
        return (std::string(""));    
    std::string str = this->request_data["Host"].substr(pos + 1, 
            (this->request_data["Host"].size() - (pos + 2)));
    return (str);
}

// getHost() is a method that gets the host. It takes no arguments and returns a string.
std::string Response::getHost()  
{
    std::string port = this->getPort();
    size_t pos = this->request_data["Host"].find_last_of(':');
    if (pos == std::string::npos)
        return (std::string(""));    
    std::string str = this->request_data["Host"].substr(0, 
            (this->request_data["Host"].size() - ((port.size()) + 2.
            )));
    return (str);
}

// BodySizeChecker() is a method that checks the body size. It takes a Parser object and a string as arguments and returns a boolean.
bool Response::BodySizeChecker(Parser &p, std::string &port)
{
    std::vector<std::string> vec =  p.get_data_from_conf(port, this->host, this->server_name, this->location , "_clientMaxBodySize", 1);

    if (std::atoi(vec[0].c_str()) <= std::atoi(this->request_data["Content-Length"].c_str()))
    {
        this->_errorCode = 413;
        this->response = this->_errorCodes[413];
        return (false);
    }
    return (true);
}

void    Response::requestHeadersCheck()
{
    std::map<std::string, std::string>::iterator transfer_it = this->request_data.find("Transfer-Encoding");
    std::map<std::string, std::string>::iterator contentLength_it = this->request_data.find("Content-Length");
    if(transfer_it != this->request_data.end() && this->request_data["Transfer-Encoding"] != "" && 
        ft_trim(this->request_data["Transfer-Encoding"], " \r\n") != "chunked" && (this->_errorCode = 501))
    {
        this->response = this->_errorCodes[501];
        return;
    }
    else if (transfer_it == this->request_data.end() && contentLength_it == this->request_data.end()
            && this->request_data["Method"] == "POST" && (this->_errorCode = 400))
        {
            this->response = this->_errorCodes[400];
            return;
        }
    else if (ForbiddenChars(std::string(this->request_data["URI"])) && (this->_errorCode = 400))
    {
        this->response = this->_errorCodes[400];
        return;
    }
    else if (this->request_data["URI"].length() > 2048 && (this->_errorCode = 414))
    {
        this->response = this->_errorCodes[414];
        return;
    }
}



std::string ft_get_file(std::string &rs)
{
    if (rs.find(".") == std::string::npos)
        return ("");
    size_t lastSlashPos = rs.rfind('/');
    std::string getp = rs.substr(lastSlashPos + 1, rs.size());
    return (getp);
}

// locationsMatching() is a method that matches the locations. It takes a Parser object, a string, and a string as arguments and returns nothing.
void    Response::locationsMatching(Parser &p, std::string &port, std::string &page_path)
{

    std::vector<std::string> locations_ = p.get_server_locations(port, host, server_name);
    int check = 0;
    int check_slash = 0;
    int check_cut_location = 1;
    size_t i = 0;

    if (page_path[page_path.size() - 1] != '/')
    {
        page_path.append("/");
        check_slash = 1;
    }
    while (check_cut_location || !page_path.empty())
    {
        i = 0;
        for(; i < locations_.size(); i++)
        {
            if (ft_trim(locations_[i], " \"\'\t") ==  page_path)
            {
                check = 1;
                check_cut_location = 0;
                this->location = page_path;
                return ;
            }
        }
        if (check_cut_location == 1)
        {
            int start = page_path.rfind('/');
            int end = page_path.size();
            if (start < end)
            {
                page_path.erase(start, end);
                if (page_path.empty())
                {
                    i = 0;
                    for(; i < locations_.size(); i++)
                    {
                        if (ft_trim(locations_[i], " \"\'\t") ==  "/")
                        {
                            check = 1;
                            check_cut_location = 0;
                            this->location = "/";
                            page_path = "/";
                            return ;
                        }
                    }
                }
                else
                {
                    i = 0;
                    for(; i < locations_.size(); i++)
                    {
                        if (locations_[i][locations_[i].size() - 1] == '/')
                        {
                            locations_[i] = locations_[i].substr(0, locations_[i].size() - 1);
                        }
                        if (ft_trim(locations_[i], " \"\'\t") ==  page_path)
                        {
                            check = 1;
                            check_cut_location = 0;
                            this->location = page_path;
                            return ;
                        }
                    }
                }
            }
        }
    }
}


int    Response::requestErrorsHandling(Parser &p, std::string &port)
{
    this->host = this->getHost();
    this->port = this->getPort();
    if (host.empty() && port.empty())
        server_name = this->request_data["Host"];
    if (this->request_data["URI"] == "")
        this->request_data["URI"] = "/";
    std::string page_path = ft_getPageName(this->request_data["URI"]);
    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(page_path);
    requestHeadersCheck();
    locationsMatching(p, port, page_path);
    if (locationRedirection(p, port) == 1 )
        return (1);
    if (!methodChecker(p, port, page_path))
        return (1);  
    return (0);
}


int Response::locationRedirection(Parser &p, std::string &port)
{
    std::string page_path = this->request_data["URI"];
    std::string httpResponse;
    
    if (page_path == "")
        page_path = "/";
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, page_path, "redirect", 1);
    if (data.size() == 1 && data[0] == "")
        return 0;
    httpResponse = "HTTP/1.1 301 Moved Permanently\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Location: " + ft_trim(data[0], " \"\'\t") + "\r\n";
    this->response = httpResponse;
    return (1);
}

bool Response::methodChecker(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, prefix, "allowed_methods", 1);
    std::string method_ = this->request_data["Method"];

    int checker = 0;

    if (method_ != "GET" && method_ != "POST" && method_ != "DELETE" && method_ != "")
    {
        this->_errorCode = 405;
        this->response = this->_errorCodes[405];
        return (false);
    }
    if (!data.empty())
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            if (ft_trim(data[i], " \"\'\t") == method_)
            {
                checker = 1;
                return (true);
            }
        }
        if (checker == 0)
        {
            if (method_ == "")
                return (true);
            this->_errorCode = 405;
            this->response = this->_errorCodes[405];
            return (false);
        }
    }
    else if (data.empty())
        return (true);
    return (false);
}

// chunked_parsing() is a method that parses the chunked data. It takes a string as an argument and returns a string.
std::string    chunked_parsing(std::string body)
{
    std::string chunked_body;
    std::string hexa_number;
    long nbr_dec;
    char* last_point;
    size_t i;
    size_t body_size;
    int j;

    if (body.empty())
        return ("");

    i = body.find("\n");
    body_size = body.size();
    hexa_number = body.substr(0, i);
    nbr_dec = std::strtol(hexa_number.c_str(), &last_point, 16);
    while (1)
    {
        j = 0;
        i = body.find("\n");
        if (i > body_size)
            break ;
        hexa_number = body.substr(0, --i);
        nbr_dec = std::strtol(hexa_number.c_str(), &last_point, 16);
        if (nbr_dec == 0)
            break ;
        body = body.substr(++i, body_size);
        while (nbr_dec > 0)
        {
            nbr_dec = nbr_dec - sizeof(body[j]);
            chunked_body.push_back(body[j++]);
        }
        body = body.substr(++j + 1 , body_size);
    }
    return (chunked_body);
}

// ft_parse_request() is a method that parses the request. It takes a string as an argument and returns nothing.
void Response::ft_parse_request(std::string request) 
{
    std::istringstream getrequestStream(request);
    std::string getrequestLine;
    std::string key;
    std::string value;
    int index = 0;
    int checker_body = 0;
    header.clear();
    size_header = 0;
    while (std::getline(getrequestStream, getrequestLine, '\n')) 
    {
        std::istringstream lines(getrequestLine);
        if (index++ == 0)
            lines >> this->request_data["Method"] >> this->request_data["URI"]
                 >> this->request_data["Type"];
        else if (getrequestLine != "\r" && getrequestLine != "")
        {
            lines >> key;
            if (getrequestLine.length() > key.length() && key.length() > 0)
            {
                key = key.substr(0, key.length() - 1);
                this->request_data[key] = ft_trim(getrequestLine.substr(key.length() + 2, getrequestLine.length()), " \t\n\"");
            }
        }
        else if (!checker_body && (checker_body = 1))
            break ;
        header.push_back(getrequestLine);
        size_header += getrequestLine.size();
    }

    if (checker_body == 1)
    {
        std::string content((std::istreambuf_iterator<char>(getrequestStream)), std::istreambuf_iterator<char>());
        if (this->request_data["Method"] == "POST")
        {
            std::stringstream file_name(content);
            if (this->request_data["Transfer-Encoding"].find("chunked") != std::string::npos)
                this->request_data["body"] = chunked_parsing(content);
            else
                this->request_data["body"] = content;
        }
    }
    parse_request_body(this->request_data["body"]);
}

std::string Response::get_filename(std::string body)
{
    std::string key = "filename=\"";
    size_t start_idx = body.find(key);
    std::string filename = "";

    if (start_idx != std::string::npos)
    {
        start_idx += key.size();
        size_t end_idx = body.find("\"", start_idx);
        if (end_idx != std::string::npos)
        {
            filename = body.substr(start_idx, end_idx - start_idx);
            return (filename);
        }
    }
    return ("");
}

std::string Response::get_content_body(std::string body)
{
    std::string key = "Content-Disposition:";
    size_t start_idx = body.find(key);
    std::string content = "";
    
    if (start_idx != std::string::npos) 
    {
        start_idx = body.find("\r\n\r\n", start_idx);
        if (start_idx != std::string::npos) 
        {
            start_idx += 4;
            size_t end_idx = body.find("\r\n", start_idx);
            if (end_idx != std::string::npos) 
            {
                content = body.substr(start_idx, (end_idx - start_idx));
                return (content);
            } 
        }
    }
    return ("");
}


std::string removeUselessData(std::string input, std::string endDelimiter) {

    std::string startDelimiter = "filename=\"";
    std::size_t startPos = input.find(startDelimiter);
    std::size_t startPosv2 = input.find("\r\n\r\n", startPos + startDelimiter.size());

    if (startPos == std::string::npos || startPosv2 == std::string::npos)
        return "";

    startPosv2 += 4;
    std::size_t endPos = input.find(endDelimiter, startPosv2);

    if (endPos == std::string::npos)
        return "";
    endPos += endDelimiter.length();
    std::string in = input.substr(startPosv2, (endPos - startPosv2 - endDelimiter.size()));
    return (in);
}

// parse_request_body() is a method that parses the request body. It takes a string as an argument and returns a string.
std::string Response::parse_request_body(std::string body)
{
    std::string contentType = this->request_data["Content-Type"];
    std::string boundaryStr = "boundary=";
    std::string val;
    std::istringstream ss(contentType);
    std::string part;
    
    while (std::getline(ss, part, ';')) 
    {
        part.erase(0, part.find_first_not_of(" \t\r\n"));
        part.erase(part.find_last_not_of(" \t\r\n") + 1);
        size_t pos = part.find(boundaryStr);
    
        if (pos != std::string::npos) 
        {
            val.append("--");
            val += part.substr(pos + boundaryStr.length());
            break;
        }
    }
    
    std::string filename = get_filename(body);
    std::string contentFile = get_content_body(body);

    this->request_data["filename"] = filename;

    if (!body.empty())
    {
        this->request_data["content_file"] = removeUselessData(body, val);
    }

    return body ;
}

// ft_getPageName() is a method that gets the page name. It takes a string as an argument and returns a string.
std::string Response::ft_getPageName(std::string pageName)
{
    size_t start = pageName.find("/", 0);
    size_t end = pageName.find("?", 0);
    std::string name ;
    if (end == std::string::npos )
        end = pageName.size();
    if (start == std::string::npos)
        return ("");
    name = pageName.substr(start, end);
    return (name);
}

std::string ft_getQuery(std::string pageName)
{
    size_t start = pageName.find("?", 0);
    if (start == std::string::npos)
        return ("");
    std::string name = pageName.substr(start + 1, (pageName.size() - start));
    return (name);
}

std::string Response::get_root_for_location(Parser &p, std::string &port, std::string &prefix)
{
    std::string page_path = prefix;
    std::vector<std::string> roots;


    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(page_path);
    roots = p.get_data_from_conf(port, this->host, this->server_name, this->location, "root", 1);
    if (!roots.empty())
        return (roots[0]);
    return ("");
}


void    Response::init_mimes(void)
{
    std::ifstream in("conf/mimes.cpp");
    std::string line;

    if(!in.is_open())
        return ;
    while(std::getline(in, line))
        mimes[line.substr(0, line.find(","))] = line.substr(line.find(",") + 1, line.size() - line.find(",") + 1);
}

// open_file() is a method that opens the file. It takes a string, an integer, a string, and a Client object as arguments and returns nothing.
void Response::open_file(std::string new_req_resource, int check_301, std::string requestedResource, Client &client)
{
    std::ifstream file(new_req_resource, std::ios::binary);
    cgi_check = 0;
    if (!file.is_open())
    {
        this->_errorCode = 404;
        this->response = this->_errorCodes[404];
    }
    else 
    {
        std::string httpResponse;
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string content_type = "text/html";
        std::string ext = requestedResource.substr(requestedResource.find_last_of(".") + 1);
        std::string cgi_response = "";
        
        if (!mimes[ext].empty())
            content_type = mimes[ext];
        else if ("."+ext  == cgi)
        {
            cgi_check = 1;
            this->request_data["PWD"] = new_req_resource;
            cgi_response = CGI::extractClientContent(client, this->request_data);
            httpResponse = this->_errorCodes[200];
            if (ERRCODE == 0)
            {
                content = cgi_response;
                content_type = "text/html";
            }
        }
        if (check_301)
        {
            this->_errorCode = 301;
            httpResponse = "HTTP/1.1 301 Moved Permanently\r\n";
            httpResponse += "Content-Type: "+content_type+"\r\n";
            httpResponse += "Location:" + requestedResource + "/" + "\r\n";
            httpResponse += "Content-Length: " + to_string(content.size()) + "\r\n";
            httpResponse += "\r\n" + content;
        }
        else
        {

            httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: "+content_type+"\r\n";
            httpResponse += "Content-Length: " + to_string(content.size()) + "\r\n";
            httpResponse += "\r\n" + content;
        }
        file.close();
        if (cgi_check == 1)
        {
            if (ERRCODE != 0)
            {
                httpResponse = this->_errorCodes[ERRCODE];
                this->_errorCode = ERRCODE;
            }
        }
        this->response = httpResponse;
    }
}

bool Response::access_checker(std::string path)
{
    const char* filename = path.c_str();
    
    if (access(filename, F_OK) == 0)
        return (true);
    else 
        return (false);
}

bool Response::look_for_index_files(std::string new_req_resource, std::string idx)
{
    if (access_checker(new_req_resource + "/" + idx))
        return (true);
    return (false);
}

bool Response::check_auto_index(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, prefix, "autoindex", 1);

    if (!data.empty())
    {
        if (data[0] == "on")
            return (true);
    }
    return (false);
}

// list_dir() is a method that lists the directory. It takes a string as an argument and returns nothing.
void Response::list_dir(std::string new_req_resource)
{
    struct stat entry_stat;
    
    if (stat(new_req_resource.c_str(), &entry_stat) == 0 )
    {
        const char* directory_path = new_req_resource.c_str();
        DIR* dir = opendir(directory_path);

        if (dir != nullptr) 
        {
            dirent* entry;
            int counter = 0 ;
            std::string httpResponse;
            std::string response;
            std::string header = "<div><h1 style=\"text-align:center\">Listing Directory : </h1><hr/>";
            while ((entry = readdir(dir)) != nullptr) 
            {
                if (counter == 0)
                    response += header;
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                    std::string ere = entry->d_name;
                    response += "<p><a href="+ere+">"+ere+"</a></p></div>\r\n\r\n";
                }
                
                counter++;
            }
            httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: text/html\r\n";
            httpResponse += "Content-Length: " + to_string(response.size()) + "\r\n";
            httpResponse += "\r\n" + response;
            this->response = httpResponse;
            closedir(dir);
        } 
    }
}

bool doesFolderExist(const std::string& foldername) {
    struct stat buffer;

    if (stat(foldername.c_str(), &buffer))
        return (true);
    return (false);
}

void Response::get_resource(Parser &p, std::string &port, Client &client)
{
    if (!auto_index)
    {
        open_file(new_req_resource, check_301, requestedResource, client);
    }
    check_error_page(p, port);
}


std::string Response::upload_checker(Parser &p, std::string &port, std::string &prefix)
{

    std::string page_path;

     if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(prefix);
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, prefix, "upload_path", 1);


    if (!data.empty())
        return (data[0]);
    return ("");
}

std::string Response::cgi_path_checker(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, prefix, "cgi", 1);

    if (!data.empty())
        return (data[0]);
    return ("");
}

// get_index_conf() is a method that gets the index configuration. It takes a Parser object, a string, a string, and a string as arguments and returns a string.
std::string Response::get_index_conf(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, prefix, "index", 1);
    std::string cutted = prefix.substr(0, prefix.size() - 1);

    if (!data.empty())
        return (data[0]);
    else
    {
        data = p.get_data_from_conf(port, this->host, this->server_name, cutted, "index", 1);
    
        if (!data.empty())
            return (data[0]);
    }
    return ("");
}

void Response::fillAndSaveFile(const std::string& fileName, const std::string& content) {

    std::string filePath = fileName;
    std::ofstream file(filePath);

    if (!file) 
        return;
    file << content;
    file.close();
}

bool Response::post_req_handler(Parser &p, std::string &port, Client &client)
{
    if (!auto_index)
    {
        open_file(new_req_resource, check_301, requestedResource , client);
        if (cgi_check == 0 && upload_checker(p, port, this->location) != "")
        {
            std::string file_path = upload_checker(p, port, this->location) +"/"+ this->request_data["filename"];
            fillAndSaveFile(file_path, this->request_data["content_file"]);
            open_file(new_req_resource, check_301, requestedResource, client);
        }
    }
    return (false);
}

// check_error_page() is a method that checks the error page. It takes a Parser object, a string, and a string as arguments and returns a boolean.
bool Response::check_error_page(Parser &p, std::string port)
{
    std::string root = get_root_for_location(p, port, this->location);
    std::vector<std::string> data = p.get_data_from_conf(port, this->host, this->server_name, this->location, "error_page", 1);
    std::string appended_root = "";

    if (!data.empty() && data.size() > 1 && this->_errorCode == std::atoi(data[0].c_str()))
    {
        appended_root = root + "/" + data[1];
        std::ifstream file(appended_root, std::ios::binary);
        std::string httpResponse;
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string content_type = "text/html";
        std::string ext = data[1].substr(data[1].find_last_of(".") + 1);

        if (!mimes[ext].empty())
            content_type = mimes[ext];

        httpResponse = "HTTP/1.1 301 Moved Permanently\r\n";
        httpResponse += "Content-Type: "+content_type+"\r\n";
        httpResponse += "Location: " + ft_trim(data[1], " \"\'\t") + "\r\n";
        file.close();
        this->response = httpResponse;
    }

    return (false);
}

// delete_handler() is a method that handles the delete request. It takes no arguments and returns nothing.
void Response::delete_handler()
{
    std::string type = getResourceType(new_req_resource);
    if (type == "file")
    {
        if (!remove(new_req_resource.c_str()))
        {
            std::cout << "File removed successfully\n";
            this->_errorCode = 200;
            this->response = this->_errorCodes[200];
        }
        else 
        {
            std::cout << "Error deleting file\n";
            this->_errorCode = 404;
            this->response = this->_errorCodes[404];
        }
    }
    else
    {
        std::cout << "Error [Delete REQUEST] don't delete a folder, specify a file please!\n";
        this->_errorCode = 404;
        this->response = this->_errorCodes[404];
    }
}



std::string Response::check_alias(Parser &p, std::string port, std::string prefix)
{
    std::string page_path = prefix;
    std::vector<std::string> roots;


    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(page_path);

    roots = p.get_data_from_conf(port, this->host, this->server_name, page_path, "alias", 1);

    if (!roots.empty())
        return (roots[0]);
    return ("");
}

// init_request_data() is a method that initializes the request data. It takes a Parser object as an argument and returns nothing.
void Response::init_request_data(Parser &p)
{
    host = this->getHost();
    port = this->getPort();
    cgi = cgi_path_checker(p, port, page_path);

    if (host.empty() && port.empty())
        server_name = this->request_data["Host"];
    else
        server_name.clear();

    requestedResource = ft_getPageName(request_data["URI"]);
    root_path = get_root_for_location(p, port, requestedResource);
    std::string alias = check_alias(p, port, this->location);

    root_path.append("/");
    if (!alias.empty())
    {
        std::string rr = requestedResource;
        rr.append("/");
        size_t start_pos = rr.find(this->location);
        if(start_pos != std::string::npos)
            rr.replace(start_pos, this->location.size(), alias);
        new_req_resource = root_path + rr;
    }
    else
        new_req_resource = root_path + requestedResource;
    
    type_ = getResourceType(new_req_resource);
    if (type_ == "file")
        fileName = ft_get_file(request_data["URI"]);
    check_301 = 0;
    auto_index = 0;
    this->p = p;
    requestedQuery = ft_getQuery(request_data["URI"]);

    this->request_data["QUERY_STRING"] = requestedQuery;
    if (type_ == "file" && fileName.find(".py"))
        this->request_data["CGI_PATH"] = new_req_resource;
}

// check_if_errors_nd_res() is a method that checks if there are errors and resources. It takes a Parser object as an argument and returns a boolean.
bool Response::check_if_errors_nd_res(Parser &p)
{
    if (requestedResource[requestedResource.size() - 1] != '/')
    {
        new_req_resource = new_req_resource + "/";
        check_301 = 1;
    }
    else
    {
        if (look_for_index_files(new_req_resource, get_index_conf(p, port, this->location)))
        {
            new_req_resource = new_req_resource + "/" + get_index_conf(p, port, this->location);
        }
        else
        {
            if (doesFolderExist(new_req_resource))
            {
                auto_index = -2; 
                this->_errorCode = 404;
                this->response = this->_errorCodes[404];
                return (false);

            }
            else if (check_auto_index(p, port, this->location))
            {
                auto_index = 1;
                list_dir(new_req_resource);
            }
            else
            {
                auto_index = -1; 
                this->_errorCode = 403;
                this->response = this->_errorCodes[403];
                return (false);
            }
        }
    }

    return (true);
}

std::string Response::createResponse(Parser &p , int type, Client &client)
{
    bool check = true;

    init_request_data(p);
    init_mimes();
    if (!BodySizeChecker(p, port))
        return ("");

    if (type_ == "folder")
        check = check_if_errors_nd_res(p);
    
    if (type == 1 && check) 
        get_resource(p, port, client);
    else if (type == 0 && check)
        post_req_handler(p, port, client);
    else if (type == 2 && check)
        delete_handler();
    return ("");
}
