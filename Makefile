NAME= webserv
# 
SRC=  Request/request.cpp Parsing\ Conf/configfile_src/ConfigfileClass.cpp Parsing\ Conf/configfile_src/LocationConfig.cpp \
	Parsing\ Conf/configfile_src/ServerConfig.cpp Parsing\ Conf/global_src/support_funcs.cpp \
	CGI/Cgi_request.cpp Server/Server.cpp Response/MIME.cpp Response/Response.cpp AutoIndex/autoindex.cpp \
	upload/upload.cpp main.cpp

Bonus_SRC=  bonus/Request/request.cpp bonus/Parsing\ Conf/configfile_src/ConfigfileClass.cpp bonus/Parsing\ Conf/configfile_src/LocationConfig.cpp \
	bonus/Parsing\ Conf/configfile_src/ServerConfig.cpp bonus/Parsing\ Conf/global_src/support_funcs.cpp \
	CGI/Cgi_request.cpp bonus/Server/Server.cpp bonus/Response/MIME.cpp bonus/Response/Response.cpp bonus/AutoIndex/autoindex.cpp \
	upload/upload.cpp bonus/main.cpp

FLAGS= -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(SRC) Makefile
	@c++ -std=c++98  $(FLAGS) $(SRC) -o $(NAME)

#bonus: $(Bonus_SRC) Makefile
#	c++ -std=c++98  $(FLAGS) $(Bonus_SRC) -o webserv_bonus
	
re: fclean $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

