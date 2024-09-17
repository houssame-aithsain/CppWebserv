#include "parsing/parser.hpp"
#include "webserver/server/Server.hpp"

// main() is the entry point of the program.
int main(int ac, char **av)
{
    Parser p;
    Server s;

    try {
        if (ac != 2)
            throw "Error: Invalid number of arguments";

        p.ft_read_nd_parse(av[1]);
        s.initializeSocket(p.wrapper);
        s.runServer(p);
    } catch (const char * error) {
        std::cout << error << std::endl;
    } catch (std::exception &e) {
        std::cout << "WEBSERV ERROR >> " << e.what()  << std::endl;
    }
}
