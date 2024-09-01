#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

typedef struct t_server_data
{
    std::vector<std::pair<std::string, std::vector<std::string> > > server;
    std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > location;
}           server_data;

class Parser
{
    protected :
        int         port;
        server_data data;

    public:
        std::vector <server_data> wrapper;
        std::vector <server_data> wrapper_v2;
        Parser();
        Parser(const Parser& p);
        Parser& operator=(const Parser& p);
        ~Parser();
        
        int getPort() const {
            return port;
        }

        void setPort(int p) {
            port = p;
        }
        
        void                        ft_check_duplicated_equals(std::string line);
        void                        ft_check_wrong_keys(std::string line, int type);
        int                         ft_check_duplicated_ports(std::vector<std::string> port);
        int                         ft_isLastServerNotEmpty(std::vector<server_data> &wrapper);
        void                        ft_check_duplicated_values(std::vector<server_data> wrapper);
        void                        ft_read_nd_parse(std::string fileName);
        void                        ft_parse_data_step2();
        std::vector<server_data>    getWrapper( void );
        std::string                 ft_trim(const std::string& str, std::string trim);
        std::vector<std::string>    ft_split_the_multiple_data(std::string value);
        std::vector<std::string>    get_data_from_conf(std::string port, std::string host, std::string server_name, std::string prefix, std::string key, int bv);
        std::vector<std::string>    get_server_locations(std::string &port, std::string server_name, std::string host);
        int                         ft_nan(std::vector<std::string> values);
        void                        check_for_default_location(void);
        void                        replace_prefix_with_alias(void);
        void                        check_error_pages(void);
        void                        check_dup_keys(void);
        void                        check_port_dup(void);
        void                        check_server_names(void);


};
