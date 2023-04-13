/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_server.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:47:51 by aptive            #+#    #+#             */
/*   Updated: 2023/04/13 15:17:40 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

void ft_socket_addr_server(int * server_fd, struct sockaddr_in * addr, int port)
{
	creation_socket_server( server_fd );
	configuration_socket_server( server_fd );
	creation_address_connexion(port, addr);
	association_socket_to_address (server_fd, addr);
	mode_listing_socket(server_fd);

	affichage_addr(*addr);

}

void creation_socket_server( int * server_fd )
{
	// Creation socket file descriptor
	*server_fd = socket (AF_INET, SOCK_STREAM, 0);
	if (*server_fd < 0)
		throw std::string("Error : Error during the creation of socket !\n");
}

void configuration_socket_server( int *server_fd )
{
	int opt = 1;
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw std::string("Error : Error during the configuration of socket !\n");
}

void creation_address_connexion( int port , struct sockaddr_in * addr)
{
	// Création de l'adresse de connexion

	std::memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;
}

void association_socket_to_address (int *server_fd, struct sockaddr_in * addr)
{
	// Association de l'adresse de connexion au socket
	if (bind(*server_fd, (struct sockaddr*)addr, sizeof(*addr)) < 0)
		throw std::string("Error : Error during the association of socket !\n");
}

void mode_listing_socket(int *server_fd)
{
	// Mise en écoute du socket
	if (listen(*server_fd, SOMAXCONN) == -1)
		throw std::string("Error : Error during the listing of socket !\n");
}

void affichage_addr(struct sockaddr_in addr)
{
	std::cout << "---------------------------------------" << std::endl;
	std::cout << "addr : " << &addr << std::endl;
	std::cout << "sin_family : " << addr.sin_family << std::endl;
	std::cout << "sin_port : " << addr.sin_port  << std::endl;
	std::cout << "sin_addr.s_addr : " << addr.sin_addr.s_addr << std::endl;
	std::cout << "---------------------------------------" << std::endl;
}
