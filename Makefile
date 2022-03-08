NAME = webserv
FLAGS = #-Wall -Wextra -Werror -std=c++98
CC = clang++
RM = rm -rf

SRCS = main.cpp \
		src/Config/FileParss.cpp \
		src/Config/serverINFO.cpp \
		src/Config/locationINFO.cpp \
		src/Request/Request.cpp \
		src/Response/Response.cpp \
		src/Cgi/cgi.cpp \
		src/Server/Server.cpp

OBJDIR = obj

OBJS = $(SRCS:.cpp=.o)

%.o:%.cpp *.hpp
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

req:
	@make re && make clean && ./webserv request
parss:
	@make re && make clean && ./webserv parssing
res:
	@make re && make clean && ./webserv response
cgi:
	@make re && make clean && ./webserv CGI


# $(OBJDIR):
#   mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: clean all