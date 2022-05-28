NAME= webserv

SRC= CGI/Cgi_request.cpp CGI/Uriparser.cpp request\ parsing\ and\ some\ basic\ response/request.cpp  \
	Parsing\ Conf/configfile_src/ConfigfileClass.cpp Parsing\ Conf/configfile_src/LocationConfig.cpp \
	Parsing\ Conf/configfile_src/ServerConfig.cpp Parsing\ Conf/global_src/support_funcs.cpp \
	Response/MIME.cpp Response/Response.cpp main.cpp

# FLAGS= -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(SRC) Makefile
	c++ -g -fsanitize=address $(FLAGS) $(SRC) -o $(NAME)