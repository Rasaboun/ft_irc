NAME = ircserv

SRC =			main.cpp \
				Utils/error.cpp \
				Utils/syntax.cpp \
				Commands/connexion_cmds.cpp \
				Commands/replies.cpp \
				Client.cpp \
				Command.cpp \
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
			$(CC)  $(HEADERS) $(SRCS) -o $(NAME) $(LIBS) 

%.o: %.cpp
				@$(CC) $(FLAGS) $(HEADERS) -c $< -o $@
				@echo "Compiled "$<" successfully!"

client:
		$(CC) $(DIR_SRCS)client.cpp -o client

std:			$(OBJS)
			$(CC)  $(HEADERS) $(SRCS) -DNAMESPACE=std -o $(NAME) $(LIBS)


ft:			$(OBJS)
			$(CC)  $(HEADERS) $(SRCS) -DNAMESPACE=ft -o $(NAME) $(LIBS) 

norme:
				norminette $(DIR_SRCS)
				norminette $(DIR_HEADERS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:			all, clean, fclean, re, bonus
