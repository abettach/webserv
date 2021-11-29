NAME = webserv
FLAGS = #-Wall -Wextra -Werror -std=c++98
CC = c++
RM = rm -rf

SRCS = main.cpp \
		src/Config/FileParss.cpp \
		src/Config/serverINFO.cpp \
		src/Config/locationINFO.cpp

OBJDIR = obj

OBJS = $(SRCS:.cpp=.o)

%.o:%.cpp *.hpp
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

# $(OBJDIR):
#   mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: clean all