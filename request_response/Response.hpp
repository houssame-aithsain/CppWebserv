#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <utility>
#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../parsing/parser.hpp"
#include <csignal>
#include <sys/event.h>
#include <limits.h>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../webserver/clientSocket/Client.hpp"
#include "../webserver/CGI/CGI.hpp"


class Response : virtual public Parser,  public CGI
{
    protected :
        std::map<std::string, std::string>  request_data;
        std::map<int, std::string>          _errorCodes;
        int                                 _errorCode;
        std::string                         response;
        std::string                         location;
        std::string                         requestedResource;
        std::string                         root_path;
        std::string                         fileName;
        std::string                         new_req_resource;
        int                                 check_301;
        int                                 auto_index;
        std::string                         port;
        std::string                         type_;
        std::string                         page_path;
        Parser                              p;
        std::string                         requestedQuery;
        std::vector<std::string>            header;
        int                                 size_header;
        std::map<int, std::string>          msg_code;
        std::string                         host;
        std::string                         server_name;
        std::string                         cgi;
        std::string                         upload_path;
        int                                 cgi_check;
        std::map<std::string, std::string>  mimes;
    public:
        Response();
        Response(const Response &rp);
        ~Response();
        Response& operator=(const Response& other);
        

        void        msg_code_arr();
        std::string check_alias(Parser &p, std::string port, std::string prefix);
        std::string get_msg(int code);
        std::string ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest);
        void        ft_parse_request(std::string request);
        void        initMapErrors();
        std::string createResponse( Parser &p, int type, Client &client);
        int         requestErrorsHandling(Parser &p, std::string &port);
        void        requestHeadersCheck();
        void        locationsMatching(Parser &p, std::string &port, std::string &page_path);
        int         locationRedirection(Parser &p, std::string &port);
        bool        methodChecker(Parser &p, std::string &port, std::string &prefix);
        std::string get_root_for_location(Parser &p, std::string &port, std::string &prefix);
        void        open_file(std::string new_req_resource, int check_301, std::string requestedResource, Client &client);
        bool        access_checker(std::string path);
        bool        look_for_index_files(std::string new_req_resource, std::string idx);
        void        get_resource(Parser &p, std::string &port, Client &client);
        bool        check_auto_index(Parser &p, std::string &port, std::string &prefix);
        std::string ft_getPageName(std::string pageName);
        void        list_dir(std::string new_req_resource);
        bool        post_req_handler(Parser &p, std::string &port, Client &client);
        std::string        upload_checker(Parser &p, std::string &port, std::string &prefix);
        std::string get_index_conf(Parser &p, std::string &port, std::string &prefix);
        std::string cgi_path_checker(Parser &p, std::string &port, std::string &prefix);
        bool        BodySizeChecker(Parser &p, std::string &port);
        std::string parse_request_body(std::string body);
        std::string get_filename(std::string body);
        std::string get_content_body(std::string body);
        void        fillAndSaveFile(const std::string& fileName, const std::string& content);
        bool        check_error_page(Parser &p, std::string port);
        void        delete_handler();
        std::string getPort();
        std::string getHost();
        void        init_request_data(Parser &p);
        bool        check_if_errors_nd_res(Parser &p);
        void        init_mimes(void);
};