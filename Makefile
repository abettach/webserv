NAME	= webserv
CC		= clang++
CFLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -f
SRC		= main.cpp \
		src/Config/FileParss.cpp \
		src/Config/serverINFO.cpp \
		src/Config/locationINFO.cpp \
		src/Request/Request.cpp \
		src/Response/Response.cpp \
		src/Cgi/cgi.cpp \
		src/Server/Server.cpp

SRCS = $(${SRC})
OBJS = ${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
%.o: %.cpp
		${CC} ${CFLAGS} $(INCLUDES) -o $@ -c $<
clean:
		${RM} ${OBJS}
fclean:	clean
		${RM} ${NAME}
re:	fclean all