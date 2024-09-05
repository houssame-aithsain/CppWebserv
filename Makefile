CC			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g

SRC			=	main.cpp webserver/server/Server.cpp parsing/parser.cpp \
				request_response/Response.cpp webserver/clientSocket/Client.cpp \
				webserver/serverSocket/ServerSocket.cpp webserver/CGI/CGI.cpp
HEADERS		=	webserver/server/Server.hpp parsing/parser.hpp request_response/Response.hpp \
				webserver/clientSocket/Client.hpp webserver/serverSocket/ServerSocket.hpp \
				webserver/CGI/CGI.hpp

NAME		=	webserv

RM			=	rm -rf

OBJS		=	$(SRC:.cpp=.o)

all			:	$(NAME)
				rm -rf ./upload

$(NAME)		:	$(OBJS) $(HEADERS)
				$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o			:	%.cpp $(HEADERS)
				$(CC) $(CXXFLAGS) -c $< -o $@

clean		:
				$(RM) $(OBJS) rm -rf ./upload

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re
