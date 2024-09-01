#include "parsing/parser.hpp"

int main(int ac, char **av)
{
    Parser p;

    try {
        if (ac != 2)
            throw "Error: Invalid number of arguments";

        p.ft_read_nd_parse(av[1]);
    } catch (const char * error) {
        std::cout << error << std::endl;
    } catch (std::exception &e) {
        std::cout << "WEBSERV ERROR >> " << e.what()  << std::endl;
    }
}
