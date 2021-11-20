NAME = webserv
FLAGS = #-Wall -Wextra -Werror -std=c++98
CC = c++
RM = rm -rf

SRCS = parssingConfigFile/main.cpp \
		parssingConfigFile/ConfigFIlePars.cpp \
		parssingConfigFile/sData.cpp \
		parssingConfigFile/location.cpp

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
		@./webserv config/config.conf| cat -e

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: clean all