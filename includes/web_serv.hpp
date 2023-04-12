/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web_serv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:14:43 by aptive            #+#    #+#             */
/*   Updated: 2023/04/12 14:55:49 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_HPP
# define WEB_SERV_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <unistd.h>

# include <stdio.h>
# include <stdlib.h>

# include <fstream>

# include <list>
# include <map>
# include <utility> // pour utiliser la paire (std::pair)

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

// configurer la connexion. On l'appelle contexte d'adressage.
// struct sockaddr_in
// {
// 	short			sin_family;
// 	unsigned short	sin_port;
// 	struct in_addr	sin_addr;
// 	char			sin_zero[8];
// };


// +----------------------------------------------------------------------------+
//                                  AFFICHAGE                                   |
// +----------------------------------------------------------------------------+
void affichage_list(std::map<std::string, std::string> list);
void affichage_socket(int client_server_fd, struct sockaddr_in client_addr);

// +----------------------------------------------------------------------------+
//                                  READ_FILE                                   |
// +----------------------------------------------------------------------------+
std::pair<std::string, std::string>	parse_to_pair (std::string str);
std::map<std::string, std::string>open_and_check_conf_file(std::string file);

// +----------------------------------------------------------------------------+
//                                    SERVER                                    |
// +----------------------------------------------------------------------------+
void todo_connexion(int fd);


// +----------------------------------------------------------------------------+
//                                  VERIF_ARG                                   |
// +----------------------------------------------------------------------------+
void						check_arg(int argc, char ** argv);

// +----------------------------------------------------------------------------+
//                                    END                                       |
// +----------------------------------------------------------------------------+



#endif
