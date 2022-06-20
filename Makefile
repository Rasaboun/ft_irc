NAME = ircserv

SRC =			main.cpp \
				Utils/error.cpp \
				Utils/syntax.cpp \
				Utils/utils.cpp \
				Commands/connexion.cpp \
				Commands/messages.cpp \
				Commands/channels.cpp \
				Commands/replies.cpp \
				Client.cpp \
				Command.cpp \
				Message.cpp \
				Channel.cpp \
				Ircserv.cpp

SRCS =			$(addprefix $(DIR_SRCS), $(SRC))

OBJS =			$(SRCS:.cpp=.o)

HEADERS =	-I $(DIR_HEADERS)

DIR_HEADERS = 		./include/ -I ./srcs/

DIR_SRCS =		./srcs/

DIR_OBJS = 		./objs/

DIR_LIBFT =		./libs/libft/

LIBS = 	




BONUS_NAME = 	

BONUS_SRC =		

BONUS_SRCS =	$(addprefix $(DIR_BONUS), $(BONUS_SRC))

BONUS_OBJS =	$(BONUS_SRCS:.cpp=.o)

BONUS_HEADERS = $(HEADERS) -I $(DIR_BONUS)include/

DIR_BONUS = 	./bonus/



RM =			rm -rf

CC =			clang++

FLAGS =			-Wall -Werror -Wextra -std=c++98



all:			$(NAME) $(BONUS)

$(NAME) :		$(OBJS)
			$(CC)  $(HEADERS) $(SRCS) -o $(NAME) $(LIBS)  && mv $(OBJS) $(DIR_OBJS)
 

%.o: %.cpp
				@$(CC) $(FLAGS) $(HEADERS) -c $< -o $@
				@echo "Compiled "$<" successfully!"

clean:
			$(RM) $(DIR_OBJS)*.o

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:			all, clean, fclean, re, bonus
