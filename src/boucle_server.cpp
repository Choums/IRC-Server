/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boucle_server.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:36:42 by aptive            #+#    #+#             */
/*   Updated: 2023/04/14 18:20:54 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/User.hpp"

//------------------------------------------------------------------------------
// Fichier en cours de tranformation en classe Server
//------------------------------------------------------------------------------

void boucle_server(int server_fd, struct sockaddr_in addr)
{
	std::vector<User> client_socket_v;

	// Initialize the read sockets set
	fd_set read_sockets;
	FD_ZERO(&read_sockets);
	FD_SET(server_fd, &read_sockets);

	int max_socket_fd = server_fd;

	// Boucle principale du serveur
	int count(0);
	while (true)
	{
		std::cout << "en attende de donnees : " << YELLOW << count << END << std::endl;
		count++;
		if (count > 20)
			exit(EXIT_SUCCESS);

		// Wait for any of the sockets to become readable
		fd_set temp = read_sockets;
		if (select(max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
			throw std::string("Error : waiting for sockets to become readable !\n");


		// traitement de l'activité sur les sockets
		gestion_new_connexion(server_fd, &temp, &max_socket_fd, &read_sockets, addr, &client_socket_v);

		affichage_vector(client_socket_v);

		// Check for data on any of the client sockets
		gestion_activite_client( &read_sockets, &client_socket_v, &temp );

	}
}


void gestion_new_connexion(int server_fd, fd_set * temp, int * max_socket_fd, fd_set * read_sockets, struct sockaddr_in addr, std::vector<User> * client_socket_v)
{
	if (FD_ISSET(server_fd, temp))
	{
		struct sockaddr_in client_address;
		int addrlen = sizeof(client_address);
		int new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);

		if (new_socket < 0)
			throw std::string("Error : Failed to accept connection !\n");

		FD_SET(new_socket, read_sockets);
		if (new_socket > *max_socket_fd)
			*max_socket_fd = new_socket;

		std::cout	<< "Nouvelle connexion : socket_fd : "
					<< new_socket << "/ ip : " << inet_ntoa(addr.sin_addr)
					<< " / port : " << ntohs(addr.sin_port)
					<< std::endl;
		client_socket_v->push_back(User(new_socket));
	}
}


void gestion_activite_client(fd_set * read_sockets, std::vector<User> * client_socket_v,fd_set * temp)
{
	int valread;
	char buffer[1025];


	for (size_t i = 0; i < client_socket_v->size(); i++)
	{
		int client_socket_fd = (*client_socket_v)[i].getFd();
		if (FD_ISSET(client_socket_fd, temp))
		{
			valread = read(client_socket_fd, buffer, 1024);
			std::string buf (buffer, valread);
			std::cout << "valread on client socket : " << valread << " / "<< client_socket_fd << std::endl;
			if (valread == 0)
			{
				std::cout << RED << "Delete \n" << (*client_socket_v)[i] << END << std::endl;
				// Client disconnected, remove from active socket set
				close(client_socket_fd);
				FD_CLR(client_socket_fd, read_sockets);
				client_socket_v->erase(client_socket_v->begin()+i);
			}
			else
			{
				// Echo back the received data
				// buffer[valread] = '\0';
				// std::cout << "Received message: " << buf << "\n";
				// send(client_socket_fd, buffer, strlen(buffer), 0);
				parsing_cmd( buffer , &((*client_socket_v)[i]));
			}
		}
	}
}
