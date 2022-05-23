NAME= webserv

SRC= Cgi_request.cpp ../request\ parsing\ and\ some\ basic\ response/request.cpp Uriparser.cpp main.cpp

# FLAGS= -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(SRC)
	clang++ -g -fsanitize=address $(FLAGS) $(SRC) -o $(NAME)