# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/05 17:28:02 by aptive            #+#    #+#              #
#    Updated: 2023/05/10 17:50:43 by chaidel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
OS		= $(shell uname)

# directories
SRCDIR		=	./src
INCDIR		=	./includes
OBJDIR		=	./obj
# CMDDIR		=	/Commands/

# src / obj files
SRC		=	main.cpp\
			affichage.cpp\
			boucle_server.cpp\
			parsing_cmd_irc.cpp\
			parsing.cpp\
			socket_server.cpp\
			User.cpp\
			Server.cpp\
			Channel.cpp\
			command_utils.cpp\
			Nick.cpp\
			Users.cpp\
			Ping.cpp\
			Join.cpp\
			Invite.cpp\
			Part.cpp\
			Mode.cpp\
			Kick.cpp\
			Privmsg.cpp\
			Topic.cpp


# nommage automatique des fichiers objets d'apres les noms des sources C
OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
# nommage automatique des fichiers de dependance d'apres les noms des sources C
SDEP	= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.d))

# ensemble des fichiers de dependance
DEP= $(SDEP)


# compiler
CC		= c++ $(CFLAGS)
CFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98 -Wshadow -Wno-shadow -MMD -fsanitize=address

all: obj $(NAME)
# @echo "COMPILATION [OK]"


obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
	$(CC) -I $(INCDIR) -o $@ -c $<

# $(OBJDIR)%.d: $(SRCDIR)/%.cpp
# 	$(CC) -MM -MD -o $@ $<


$(NAME): $(OBJ)
	$(CC) $(OBJ) -lm -o $(NAME)

clean:
	rm -rf $(OBJDIR) $(DEP)

fclean: clean
	rm -rf $(NAME)

re: fclean
	make all

# inclusion des dependances
-include $(DEP)
