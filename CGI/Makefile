NAME= webserv

SRC= Cgi_request.cpp ../request\ parsing\ and\ some\ basic\ response/request.cpp Uriparser.cpp \
	../Parsing\ Conf/configfile_src/ConfigfileClass.cpp ../Parsing\ Conf/configfile_src/LocationConfig.cpp \
	../Parsing\ Conf/configfile_src/ServerConfig.cpp main.cpp

# FLAGS= -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(SRC)
	clang++ -g -fsanitize=address $(FLAGS) $(SRC) -o $(NAME)