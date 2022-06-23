# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wluong <wluong@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/18 18:53:23 by jdidier           #+#    #+#              #
#    Updated: 2022/06/23 02:16:29 by wluong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

SRC			= 	main.cpp \
				srcs/Socket.cpp \
				srcs/Server.cpp \
				srcs/Service.cpp \
				srcs/parsing_conffile.cpp \
				srcs/parsing_server.cpp \
				srcs/parsing_request.cpp \
				srcs/utils_parsing.cpp \
				srcs/parsing.cpp

OBJ			= $(SRC:.cpp=.o)
DEP			= $(OBJ:.o=.d)

CC			= c++
FLAGS		= -Wall -Werror -Wextra -std=c++98 #	-g3 -fsanitize=address

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(FLAGS) $(OBJ) -o $(NAME)

-include $(DEP)

%.o:%.cpp
	$(CC) -MMD $(FLAGS) -c $< -o $@

clean:
			rm -f $(OBJ) $(DEP)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re