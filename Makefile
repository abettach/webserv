NAME = webserv
FLAGS = #-Wall -Wextra -Werror -std=c++98
CC = c++
RM = rm -rf

SRCS = parss/main.cpp \
		parss/ConfigFIlePars.cpp \
		parss/sData.cpp \
		parss/location.cpp

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
	@$(RM) parss/*.o

fclean: clean
	@$(RM) $(NAME)

re: clean all