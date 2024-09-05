/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:53:27 by gothmane          #+#    #+#             */
/*   Updated: 2024/04/04 01:09:45 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parser.hpp"
#include "webserver/server/Server.hpp"

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
