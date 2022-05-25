# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/25 15:20:55 by mbrija            #+#    #+#              #
#    Updated: 2022/05/25 16:13:56 by mbrija           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS= main.cpp configfile_src/*.cpp global_src/*.cpp

FLAGS=-Wextra -Wall -Werror 

NAME=test_conf

$(NAME):
	c++ $(FLAGS) $(SRCS) -o $(NAME)

all: $(NAME) $(SRCS)

re: fclean $(NAME)

clean:
	rm -rf $(NAME) $(WRONG)

fclean: clean