NAME= webserv
# 
SRC=  request.cpp ../Parsing\ Conf/configfile_src/ConfigfileClass.cpp ../Parsing\ Conf/configfile_src/LocationConfig.cpp \
	../Parsing\ Conf/configfile_src/ServerConfig.cpp ../Parsing\ Conf/global_src/support_funcs.cpp \
	../CGI/Cgi_request.cpp ../Server/Server.cpp ../CGI/Uriparser.cpp  ../Response/MIME.cpp ../Response/Response.cpp ../AutoIndex/autoindex.cpp \
	../upload/upload.cpp main.cpp

# FLAGS= -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(SRC) Makefile
	c++ -g -fsanitize=address $(FLAGS) $(SRC) -o $(NAME)