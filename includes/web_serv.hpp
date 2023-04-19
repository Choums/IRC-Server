/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web_serv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:14:43 by aptive            #+#    #+#             */
/*   Updated: 2023/04/19 15:17:40 by root             ###   ########.fr       */
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
# include <sstream>


# include <list>
# include <vector>
# include <map>
# include <utility> // pour utiliser la paire (std::pair)

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

#include "User.hpp"
#include "Channel.hpp"

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define END "\033[0m"

enum Command {
	Nick = 1,
	Join = 2,
	Unknown = 0
};

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
//                                BOUCLE_SERVER                                 |
// +----------------------------------------------------------------------------+
void boucle_server(int server_fd, struct sockaddr_in addr);

// +----------------------------------------------------------------------------+
//                                   SERVER                                     |
// +----------------------------------------------------------------------------+
void todo_connexion(int fd);

// +----------------------------------------------------------------------------+
//                               PARSING_CMD_IRC                                |
// +----------------------------------------------------------------------------+

// +----------------------------------------------------------------------------+
//                                 PARSING_CPP                                  |
// +----------------------------------------------------------------------------+
std::vector<std::string> split_string(std::string str);

void parsing(int argc, char ** argv, int * port, std::string * password);
void check_arg(int argc, char ** argv);


// +----------------------------------------------------------------------------+
//                               SOCKET_SERVER_CPP                              |
// +----------------------------------------------------------------------------+
void ft_socket_addr_server(int * server_fd, struct sockaddr_in * addr, int port);
void creation_socket_server( int * server_fd );
void configuration_socket_server( int *server_fd );
void creation_address_connexion( int port , struct sockaddr_in * addr);
void association_socket_to_address (int *server_fd, struct sockaddr_in * addr);
void mode_listing_socket(int *server_fd);
void affichage_addr(struct sockaddr_in addr);

// +----------------------------------------------------------------------------+
//                                    END                                       |
// +----------------------------------------------------------------------------+

template <typename T>
void affichage_vector(std::vector<T> v)
{
	std::cout << RED << "*******************" << std::endl;
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << GREEN << "-------------------" << std::endl;
		std::cout << "vector : " << i << std::endl << v[i];
		std::cout << "-------------------" << std::endl;
	}
	std::cout << RED << "*******************" << END << std::endl;

}

#endif
