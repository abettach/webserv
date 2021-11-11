NAME = webserv
FLAGS = -Wall -Wextra -Werror -std=c++98
CC = clang++
RM = rm -rf

SRCS = parssingConfigFile/main.cpp \
		parssingConfigFile/ConfigFIlePars.cpp

OBJS = $(SRCS:.cpp=.o)

%.o:%.cpp *.hpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

test:
		make re
		@make clean
		@echo " "
		@./webserv | cat -e

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: clean all