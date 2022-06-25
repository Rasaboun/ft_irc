NAME = ircserv

SRC =			main.cpp \
				error.cpp \
				syntax.cpp \
				utils.cpp \
				connexion.cpp \
				messages.cpp \
				channels.cpp \
				server.cpp \
				replies.cpp \
				Client.cpp \
				Command.cpp \
				Message.cpp \
				Channel.cpp \
				Ircserv.cpp

SRCS =			$(addprefix $(DIR_SRCS)/, $(SRC))

OBJS =			$(addprefix $(DIR_OBJS)/, $(SRC:%.cpp=%.o))

HEADERS =	-I include/

DIR_SRCS =		srcs

DIR_OBJS = 		objs

RM =			rm -rf

CC =			clang++

FLAGS =			-Wall -Werror -Wextra -std=c++98



all:			$(NAME)

$(NAME) :		$(OBJS)
			$(CC)  $(HEADERS) $(SRCS) -o $(NAME)
 

$(OBJS): $(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp | $(DIR_OBJS)
	@clang++ $(FLAGS) $(HEADERS) -g -c $< -o $@
	@echo Compiling : $< "Okay"

$(DIR_OBJS):
	@mkdir $(DIR_OBJS)

clean:
			$(RM) $(DIR_OBJS)/*.o

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:			all, clean, fclean, re, bonus
