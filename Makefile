# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aptive <aptive@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/05 17:28:02 by aptive            #+#    #+#              #
#    Updated: 2023/04/12 18:57:25 by aptive           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
OS		= $(shell uname)

# directories
SRCDIR		=	./src
INCDIR		=	./includes
OBJDIR		=	./obj

# src / obj files
SRC		=	main.cpp\
			affichage.cpp\
			read_file.cpp\
			server.cpp\
			verif_arg.cpp\
			User.cpp\


# nommage automatique des fichiers objets d'apres les noms des sources C
OBJ		= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
# nommage automatique des fichiers de dependance d'apres les noms des sources C
SDEP	= $(addprefix $(OBJDIR)/,$(SRC:.cpp=.d))

# ensemble des fichiers de dependance
DEP= $(SDEP)


# compiler
CC		= c++ $(CFLAGS)
CFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98 -Wshadow -Wno-shadow -fsanitize=address -MMD

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
