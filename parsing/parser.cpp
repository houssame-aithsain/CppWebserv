#include "parser.hpp"

// ft_trim() is a method that trims the string. It takes a string and a string as arguments and returns a string.
std::string Parser::ft_trim(const std::string& str, std::string trim)
{
    size_t first = str.find_first_not_of(trim);
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(trim);
    return str.substr(first, (last - first + 1));
}

int ft_checkisNumber(const std::string& s)
{
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) 
            return -1;
    }
    return 1;
}

// ft_nan() is a method that checks if the value is a number. It takes a vector of strings as an argument and returns an integer.
int Parser::ft_nan(std::vector<std::string> values)
{
    for (std::size_t k = 0; k < values.size(); ++k) 
    {
        if (ft_checkisNumber(ft_trim(values[k], " \t")) == -1)
            return (-1);
    }
    return (1);
}

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser &p)
{
    (void) p;
}

Parser &Parser::operator=(const Parser &pr)
{
    (void) pr;

    return (*this);
}

// ft_count_special_char() is a method that counts the special characters. It takes a string and a character as arguments and returns an integer.
int ft_count_special_char(std::string line, char car)
{
    size_t i = 0;
    int    counter = 0;

    for (; i < line.size(); i++)
    {
        if (line[i] == car)
            counter++;
    }
    return (counter);
}

int ft_check_whitespaces(std::string str)
{
    size_t checker = 0;
    size_t i = 0;
    for (;i < str.size(); i++)
    {
        if (str[i] == ' ' || str[i] == '\t')
            checker++;
    }
    if (checker == i)
        return (0);
    return (1);
}

// ft_split_the_multiple_data() is a method that splits the multiple data. It takes a string as an argument and returns a vector of strings.
std::vector<std::string>  Parser::ft_split_the_multiple_data(std::string value)
{
    int open_brackets = 0;
    int close_brackets = 0;
    std::stringstream ss(value);
    std::vector<std::string> v ;
    size_t i = 0;

    if (value[i] == '[')
        open_brackets = 1;

    i = value.size() - 1;

    if (open_brackets == 1 && value[i] == ']')
        close_brackets = 1;

    if (open_brackets == 1 && close_brackets == 1)
    {
        std::string trimmedValue = ft_trim(value, "[]");
        char* str = new char[trimmedValue.length() + 1];
        std::strcpy(str, trimmedValue.c_str());  
        
        const char *token = strtok(str, ",");
    
        while (token != NULL) {
            v.push_back(ft_trim(std::string(token), " \t").c_str());
            token = strtok(NULL, ",");
        }

        delete[] str;
        return (v);
    }
    else if (open_brackets == 0 && close_brackets == 0)
    {
        if (value != "")
            v.push_back(ft_trim(value, " \t"));
        else
            throw "ERROR IN CONFIG FILE"; // throw exception of brackets
        return (v);
    }
    
    std::string no_brackets_value = value.substr(1, value.size() - 2); 
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, ','))
    {
        item = item.substr(1, item.size() - 2);
        result.push_back(ft_trim(item, " \t"));
    }
    return (result);
}

// ft_read_nd_parse() is a method that reads and parses the configuration file. It takes a string as an argument and returns nothing.
void Parser::ft_read_nd_parse(std::string fileName)
{
    std::ifstream in(fileName);
    std::string line;
    int server_side = 0;
    int location_side = 0;
    int server_id = 0;
    size_t j = -1;

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (ft_trim(line, " \t") == "[[server]]" )
            {
                if (server_side > 0)
                {
                    wrapper.push_back(data);
                    data.server.clear();
                    data.location.clear();                
                }
                server_side++;
                location_side = 0;
                j = -1;
                continue;
            }
            else if (ft_trim(line, " \t") == "[[server.location]]")
            {
                if (server_side < 1)
                {
                    std::cout << "Error: Missing server !\n";
                    throw "ERROR IN CONFIG FILE";
                }
                server_id = server_side;
                location_side++;
                j++;
                continue;
            }
            if (server_id != server_side && location_side == 0)
            {
                ft_check_duplicated_equals(line);
                if (line.find("=") == std::string::npos)
                {
                    if (ft_check_whitespaces(line) == 1 && line != "")
                    {
                        std::cout << "Error: Can't that mate in the server data => [" << line << "]" << std::endl;
                        throw "ERROR IN CONFIG FILE";
                    }
                    continue;
                }
                std::string key = ft_trim((line.substr(0, line.find("="))), " \t");
                if (key == "")
                    continue;
                ft_check_wrong_keys(key, 0);
                std::vector<std::string> value;
                value = ft_split_the_multiple_data(ft_trim(line.substr(line.find("=") + 1, line.size()), " \t"));
                
                if (key == "port" && (ft_nan(value) == -1))
                {
                    std::cout << "Error: Port value\n";
                    throw "ERROR IN CONFIG FILE";
                }
                data.server.push_back(std::make_pair(key, value));
            }
            else if (location_side > 0)
            {
                ft_check_duplicated_equals(line);
                std::string key = ft_trim(line.substr(0, line.find("=")), " \t");
                if (line.find("=") == std::string::npos)
                {
                    if (ft_check_whitespaces(line) == 1 && line != "")
                    {
                        std::cout << "Error: Can't that mate in the location data => " << line << std::endl;
                        throw "ERROR IN CONFIG FILE";
                    }
                    continue;
                }
                ft_check_wrong_keys(key, 1);
                std::vector<std::string> value;
                value = ft_split_the_multiple_data(ft_trim(line.substr(line.find("=") + 1, line.size()), " \t"));

                if (j >= data.location.size()) {
                    data.location.resize(j+1);
                }
                if ((key == "prefix") && value[0][value[0].size() - 1] != '/' )
                {
                    value[0].append("/");
                }
                data.location.at(j).push_back(std::make_pair(key, value));
            }
        }
        if (server_side > 0)
            wrapper.push_back(data);
    }
    else
        std::cout << "Unable to open file\n";

    check_for_default_location();
    ft_check_duplicated_values(wrapper);
    check_error_pages();
    check_dup_keys();
    check_port_dup();
    check_server_names();
}

void Parser::ft_check_duplicated_equals(std::string line)
{
    int checker = 0;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (checker == 0 && line[i] == '=')
            checker++;
        else if (checker != 0 && line[i] == '=')
        {
            std::cout << "Error: Duplicated equals !\n";
            throw "ERROR IN CONFIG FILE";
        }
    }
}

// ft_ckeck_wrong_keys() is a method that checks for wrong keys. It takes a string and an integer as arguments and returns nothing.
void Parser::ft_check_wrong_keys(std::string key, int type)
{
    if (type == 0)
    {
        if (key != "port" && key != "host" && key != "root" && key != "server_name" && key != "allowed_methods" && key != "autoindex"
            && key != "index" && key != "error_page" && key != "_clientMaxBodySize" && key != "upload_path" && key != "host" && key != "server_name")
        {
            std::cout << "Error: Wrong value in the server side of config file ! Check your value  "<< key << "\n" ;
            throw "ERROR IN CONFIG FILE";
        }
    }
    else if (type == 1)
    {
        if (key != "prefix" && key != "allowed_methods" && key != "root" && key != "autoindex"
            && key != "index" && key != "error_page" && key != "_clientMaxBodySize" && key != "alias" && key != "cgi" && key != "upload_path" && key != "redirect" )
        {
            std::cout << "Error: Wrong value in the location side of config file ! Check your value  "<< key << "\n" ;
            throw "ERROR IN CONFIG FILE";
        }
    }
}

int Parser::ft_check_duplicated_ports(std::vector<std::string> port)
{
    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it) {
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt) {
        std::string key = jt->first;
        std::vector<std::string> values = jt->second;

        for (std::size_t k = 0; k < values.size(); ++k) {
            if (port[0] == values[k])
                return (1);
        }
    }
    }
    return (0);
}

int Parser::ft_isLastServerNotEmpty(std::vector<server_data> &wrapper)
{
    if (!wrapper.empty() && !wrapper.back().server.empty())
        return 1;
    return -1;
}

// ft_check_duplicated_values() is a method that checks for duplicated values. It takes a vector of server_data as an argument and returns nothing.
void Parser::ft_check_duplicated_values(std::vector<server_data> wrapper)
{
    if (wrapper.empty())
    {
        throw "Error: Wrapper Already Empty";
    }
    for (size_t i = 0; i < wrapper.size(); i++)
    {
        for (size_t j = 0; j < wrapper[i].server.size(); j++)
        {
            std::string key_to_find = wrapper[i].server[j].first;
            int count = 0;
            for (size_t k = 0; k < wrapper[i].server.size(); k++)
            {
                std::string key_two = wrapper[i].server[k].first;
                if (key_to_find == key_two)
                    count++;
                if (count > 1)
                {
                    std::cout << "Error: Duplicatde values in the config file\n";
                    throw "ERROR IN CONFIG FILE";
                }
            }
        }
        if (wrapper[i].location.size() == 1)
            return ;
        for (size_t j = 0; j < wrapper[i].location.size(); j++)
        {
            for (std::size_t e = 0; e < wrapper[i].location[j].size(); ++e) 
            {
                std::string key_to_find = wrapper[i].location[j][e].first;
                int count = 0;
                for (std::size_t k = 0; k < wrapper[i].location[j].size(); ++k) 
                {
                    std::string key_two = wrapper[i].location[j][k].first;
                    if (key_to_find == key_two && key_two != "")
                        count++;
                    if (count > 1)
                        throw "ERROR IN CONFIG FILE";
                }
            }
        }
    }
}

std::vector<server_data> Parser::getWrapper( void ) {return (wrapper);}

// check_for_default_location() is a method that checks for the default location. It takes no arguments and returns nothing.
void    Parser::check_for_default_location(void)
{
    size_t i = 0;
    int checker = 0;
    std::vector<std::pair<std::string, std::vector<std::string> > >  location;

    for (; i < this->wrapper.size(); i++)
    {
        checker = 0;
        for (size_t l = 0; l < wrapper[i].location.size(); l++)
        {
            size_t s = 0;
            for (; s < wrapper[i].location[l].size(); s++)
            {
                if (wrapper[i].location[l][s].first == "prefix" 
                    && ft_trim(wrapper[i].location[l][s].second[0], " \"\'\t") == "/")
                {
                    checker = 1;
                    break;
                }
            }
        }
        if (checker == 0)
        {
            std::vector<std::string> v;

            v.push_back("/");
            location.push_back(std::make_pair("prefix", v));
            wrapper[i].location.push_back(location);
        }
    }
}

// get_data_from_conf() is a method that gets data from the configuration. It takes a string, a string, a string, a string, a string, and an integer as arguments and returns a vector of strings.
std::vector<std::string> Parser::get_data_from_conf(std::string port, std::string host, std::string server_name, std::string prefix, std::string key, int bv)
{
    size_t i = 0;
    size_t k = 0; 
    size_t checker = 0;
    size_t check_host = 0;
    size_t check_port = 0;
    std::vector<std::string> default_value;
    std::vector<std::string> empty_for_empty;
    
    empty_for_empty.push_back("");
    if (prefix[prefix.size() - 1] != '/')
        prefix.append("/");
    if ((port.empty() || host.empty()) && server_name.empty())   
        return empty_for_empty;
    for (; i < wrapper.size(); i++)
    {
        checker = 0;
        for (size_t j = 0; j < wrapper[i].server.size(); j++)
        {
            checker = 0;
            if (!server_name.empty() &&
                    ft_trim(wrapper[i].server[j].first, " \"\'\t") == "server_name" 
                    && ft_trim(wrapper[i].server[j].second[0], " \"\'\t\n\r") == ft_trim(server_name, " \"\'\t\n\r"))
            {
                k = j;
                checker = 1;
              
                for (size_t e = 0; e < wrapper[i].server.size(); e++)
                    if (ft_trim(wrapper[i].server[e].first, " \"\'\t") == ft_trim(key, " \"\'\t"))
                        default_value = wrapper[i].server[e].second;
                break;
            }
            else if (server_name.empty() && ft_trim(wrapper[i].server[j].first, " \"\'\t") == "port" )
            {
                for (size_t w = 0 ; w < wrapper[i].server[j].second.size(); w++ )
                {
                    if (ft_trim(wrapper[i].server[j].second[w], " \"\'\t") == port)
                    {
                      check_port = 1;
                      break;
                    }
                }
            }
            else if (server_name.empty() && ft_trim(wrapper[i].server[j].first, " \"\'\t") == "host")
            {
                for (size_t w = 0 ; w < wrapper[i].server[j].second.size(); w++ )
                {
                    if (ft_trim(wrapper[i].server[j].second[w], " \"\'\t") == host)
                    {
                        check_host = 1;
                        break;
                    }
                }
            }
            else if (check_port == 1 && check_host == 1)
            {
                k = j;
                checker = 1;
                check_host = 0;
                check_port = 0;
                for (size_t e = 0; e < wrapper[i].server.size(); e++)
                    if (ft_trim(wrapper[i].server[e].first, " \"\'\t") == ft_trim(key, " \"\'\t"))
                        default_value = wrapper[i].server[e].second;
                break;
            }
        }


        if (!bv && !default_value.empty())
        {
            return (default_value);
        }
        else if (i == wrapper.size() - 1 && checker != 1)
        {
            checker = 2;
        }
        if (checker == 1 && bv)
        { 
            for (size_t l = 0; l < wrapper[i].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[i].location[l].size(); s++)
                {
                    if (wrapper[i].location[l][s].first == "prefix" 
                        && ft_trim(wrapper[i].location[l][s].second[0], " \"\'\t") == prefix)
                    {
                        for (size_t k = 0; k < wrapper[i].location[l].size(); k++)
                        { 
                            if (wrapper[i].location[l][k].first == key)
                            {
                                return (wrapper[i].location[l][k].second);
                            }
                        }
                        break;
                    }

                }
            }
            if (!default_value.empty())
                return (default_value);
            else
                return (empty_for_empty); // JUST ADDED THAT to check if the key is not found in those locations don't go past other ones
        }

        else if (checker == 2 && bv)
        { 
            for (size_t e = 0; e < wrapper[0].server.size(); e++)
                if (ft_trim(wrapper[0].server[e].first, " \"\'\t") == ft_trim(key, " \"\'\t"))
                    default_value = wrapper[0].server[e].second;
                    
            for (size_t l = 0; l < wrapper[0].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[0].location[l].size(); s++)
                {
                    if (wrapper[0].location[l][s].first == "prefix" 
                        && ft_trim(wrapper[0].location[l][s].second[0], " \"\'\t") == prefix)
                    {                        
                        for (size_t k = 0; k < wrapper[0].location[l].size(); k++)
                        {   
                            if (wrapper[0].location[l][k].first == key)
                            {
                                return (wrapper[0].location[l][k].second);
                            }
                        }
                        break;
                    }

                }
            }
            if (!default_value.empty())
            {
                return (default_value);
            }
            else
                return (empty_for_empty); // JUST ADDED THAT to check if the key is not found in those locations don't go past other ones
        }

    }
    return (empty_for_empty);
}

// get_server_locations() is a method that gets the server locations. It takes a string, a string, and a string as arguments and returns a vector of strings.
std::vector<std::string> Parser::get_server_locations(std::string &port,  std::string host, std::string server_name)
{
    size_t i = 0;
    size_t k = 0;
    int checker = 0;
    std::vector<std::string> default_value;
    std::vector<std::string> prefixes;
    int check_port = 0;
    int check_host = 0;
    int inside = 0;
    
    for (; i < wrapper.size(); i++)
    {
        if (inside == 1)
            return (prefixes);
        for (size_t j = 0; j < wrapper[i].server.size(); j++)
        {
            checker = 0;
            if (!server_name.empty() &&
                    ft_trim(wrapper[i].server[j].first, " \"\'\t") == "server_name" 
                    && ft_trim(wrapper[i].server[j].second[0], " \"\'\t") == ft_trim(server_name, " \"\'\t\r\n"))
            {
                checker = 1;
                k = j;
                break;
            }
            else if (server_name.empty() && ft_trim(wrapper[i].server[j].first, " \"\'\t") == "port" )
            {

                for (size_t w = 0 ; w < wrapper[i].server[j].second.size(); w++ )
                {
                    
                    if (ft_trim(wrapper[i].server[j].second[w], " \"\'\t") == port)
                    {
                      check_port = 1;
                      break;
                    }
                }
            }
            else if (server_name.empty() && ft_trim(wrapper[i].server[j].first, " \"\'\t") == "host")
            {
                for (size_t w = 0 ; w < wrapper[i].server[j].second.size(); w++ )
                {
                    if (ft_trim(wrapper[i].server[j].second[w], " \"\'\t") == host)
                    {
                        check_host = 1;
                        break;
                    }
                }
            }
            else if (check_port == 1 && check_host == 1)
            {
                checker = 1;
                k = j;
                check_host = 0;
                check_port = 0;
                break;
            }
            else if (i == wrapper.size() - 1 &&  checker != 1)
            {
                checker = 2;
                break;
            }
        }
        if (checker == 1) 
        {
            for (size_t l = 0; l < wrapper[i].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[i].location[l].size(); s++)
                {
                    if (wrapper[i].location[l][s].first == "prefix" )
                    {
                        prefixes.push_back(wrapper[i].location[l][s].second[0]);
                        inside = 1;
                        break;
                    }
                }
            }
        }
        else if (checker == 2)
        {
            for (size_t l = 0; l < wrapper[0].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[0].location[l].size(); s++)
                {
                    if (wrapper[0].location[l][s].first == "prefix" )
                    {
                        prefixes.push_back(wrapper[0].location[l][s].second[0]);
                        inside = 1;
                        break;
                    }
                }
            }
        }
    }
    return (prefixes);
}

// check_server_names() is a method that checks if there are duplicated server names. It takes no arguments and returns nothing.
void Parser::check_server_names(void)
{
    std::vector<std::string> server_names;
    int check = 0;
    int check_methodes = 0;
    int check_maxbs = 0;
    int check_root = 0;
    int check_index = 0;
    int check_auto_index = 0;
    int check_cgi = 0;
    int i = 0;

    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it)
    {
        check = 0;
        check_methodes = 0;
        check_maxbs = 0;
        check_root = 0;
        check_index = 0;
        check_auto_index = 0;
        check_cgi = 0;
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt)
        {
            std::string key = jt->first;
            std::vector<std::string> values = jt->second;

            if (key == "server_name")
            {
                server_names.push_back(values[0]);
                check = 1;
            }
            else if (key == "allowed_methods")
                check_methodes = 1;
            else if (key == "_clientMaxBodySize")
                check_maxbs = 1;
            else if (key == "root")
                check_root = 1;
            else if (key == "index")
                check_index = 1;
            else if (key == "autoindex")
                check_auto_index = 1;
            else if (key == "cgi")
                check_cgi = 1;
        }
        if (check == 0)
        {
           std::vector<std::string> v;

            v.push_back("default.com");
            wrapper[i].server.push_back(std::make_pair("server_name", v));
        }
        if (check_methodes == 0)
        {
            std::vector<std::string> v;

            v.push_back("GET");
            v.push_back("POST");
            v.push_back("DELETE");
            wrapper[i].server.push_back(std::make_pair("allowed_methods", v));
        }
        if (check_maxbs == 0)
        {
            std::vector<std::string> v;

            v.push_back("1024");
            wrapper[i].server.push_back(std::make_pair("_clientMaxBodySize", v));
        }
        if (check_root == 0)
        {
            std::vector<std::string> v;

            v.push_back("/Users/gothmane/Desktop/v01");
            wrapper[i].server.push_back(std::make_pair("root", v));
        }
        if (check_index == 0)
        {
            std::vector<std::string> v;

            v.push_back("index.html");
            wrapper[i].server.push_back(std::make_pair("index", v));
        }
        if (check_auto_index == 0)
        {
            std::vector<std::string> v;

            v.push_back("off");
            wrapper[i].server.push_back(std::make_pair("autoindex", v));
        }
        if (check_cgi == 0)
        {
            std::vector<std::string> v;

            v.push_back(".py");
            wrapper[i].server.push_back(std::make_pair("cgi", v));
        }
        i++;
    }
    for (size_t i = 0; i < server_names.size(); ++i)
    {
        for (size_t j = i + 1; j < server_names.size(); ++j)
        {
            if (server_names[i] == server_names[j])
            {
                std::cout << "Error: can't have duplicated server_name" << std::endl;
                throw "ERROR IN CONFIG FILE";
            }
        }
    }
}

// check_port_dup() is a method that checks if there are duplicated ports. It takes no arguments and returns nothing.
void Parser::check_port_dup(void)
{
    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it)
    {
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt)
        {
            std::string key = jt->first;
            std::vector<std::string> values = jt->second;

            if (key == "port")
            {
                for (size_t i = 0; i < values.size(); ++i)
                {
                    for (size_t j = i + 1; j < values.size(); ++j)
                    {
                        if (values[i] == values[j])
                        {
                            std::cout << "Error: can't have duplicated port in same server" << std::endl;
                            throw "ERROR IN CONFIG FILE";
                        }
                    }
                }
            }
        }
    }
}

// check_dup_keys() is a method that checks if there are duplicated keys. It takes no arguments and returns nothing.
void Parser::check_dup_keys(void)
{
    std::vector<std::string> keys;
    
    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it)
    {
        keys.clear();
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt)
        {
            std::string key = jt->first;
            
            keys.push_back(key);
        }
        for (size_t i = 0; i < keys.size(); ++i)
        {
            for (size_t j = i + 1; j < keys.size(); ++j)
            {
                if (keys[i] == keys[j])
                {
                    std::cout << "Error: can't have duplicated keys in the server" << std::endl;
                    throw "ERROR IN CONFIG FILE";
                }
            }
        }
        for (std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > >::iterator kt = it->location.begin(); kt != it->location.end(); ++kt)
        {
            keys.clear();
            for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator lt = kt->begin(); lt != kt->end(); ++lt)
            {
                std::string key = lt->first;

                keys.push_back(key);
            }
            for (size_t i = 0; i < keys.size(); ++i)
            {
                for (size_t j = i + 1; j < keys.size(); ++j)
                {
                    if (j == i + 1)
                        return ;
                    if (keys[i] == keys[j])
                    {
                        std::cout << "Error: can't have duplicated keys in the location " << keys[i] << std::endl;
                        throw "ERROR IN CONFIG FILE";
                    }
                }
            }
        }
       
    }
}

// check_error_pages() is a method that checks if there are error pages. It takes no arguments and returns nothing.
void Parser::check_error_pages(void)
{
    std::vector<std::string> server_names;
    std::vector<std::string> allowed_error_codes;

    allowed_error_codes.push_back("400");
    allowed_error_codes.push_back("401");
    allowed_error_codes.push_back("403");
    allowed_error_codes.push_back("404");
    allowed_error_codes.push_back("500");
    allowed_error_codes.push_back("502");
    allowed_error_codes.push_back("503");
    allowed_error_codes.push_back("504");
    
    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it)
    {
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt)
        {
            std::string key = jt->first;
            std::vector<std::string> values = jt->second;

            if (key == "error_page")
                server_names.push_back(values[0]);
        }
        for (std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > >::iterator kt = it->location.begin(); kt != it->location.end(); ++kt)
        {
            for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator lt = kt->begin(); lt != kt->end(); ++lt)
            {
                std::string key = lt->first;
                std::vector<std::string> values = lt->second;

               if (key == "error_page")
                server_names.push_back(values[0]);
            }
        }
    }
    size_t check = 0;
    
    for (size_t i = 0; i < server_names.size(); ++i)
    {
       for (size_t j = 0; j < allowed_error_codes.size(); j++)
       {
            if (server_names[i] == allowed_error_codes[j])
            {
                check++;
                break;
            }
       }
    }
    if (check != server_names.size())
    {
        throw "ERROR IN CONFIG FILE";
    }
}
