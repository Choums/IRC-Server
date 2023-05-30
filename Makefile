# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/05 17:28:02 by aptive            #+#    #+#              #
#    Updated: 2023/05/30 18:38:18 by chaidel          ###   ########.fr        #
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
			parsing.cpp\
			User.cpp\
			Server.cpp\
			Channel.cpp\
			command_utils.cpp\
			commandes/Users.cpp\
			commandes/Nick.cpp\
			commandes/Ping.cpp\
			commandes/Join.cpp\
			commandes/Invite.cpp\
			commandes/Part.cpp\
			commandes/Mode.cpp\
			commandes/Kick.cpp\
			commandes/Privmsg.cpp\
			commandes/Topic.cpp\
			commandes/Notice.cpp\
			commandes/List.cpp\
			# boucle_server.cpp\
			# parsing_cmd_irc.cpp\
			# socket_server.cpp\


# nommage automatique des fichiers objets d'apres les noms des sources C
OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
# nommage automatique des fichiers de dependance d'apres les noms des sources C
SDEP	= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.d))

# ensemble des fichiers de dependance
DEP= $(SDEP)


# compiler
CC		= c++ $(CFLAGS)
CFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98 -Wshadow -Wno-shadow -MMD #-fsanitize=address

all: obj $(NAME)
# @echo "COMPILATION [OK]"


obj:
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/commandes


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
