/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/12 16:36:57 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

int main(int argc, char ** argv)
{
	// std::string port;
	std::string password;

	int port;
	int server_fd(-1);
	char buffer[1025];
	try
	{
		check_arg(argc, argv);

		port = atoi(argv[1]);
		password = argv[2];

		std::cout << "Port / Password : " << port << " / " << password << std::endl;

		// Creation socket file descriptor
		server_fd = socket (AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0)
			throw std::string("Error : Error during the creation of socket !\n");



		// configuration du socket
		int opt = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
			throw std::string("Error : Error during the configuration of socket !\n");


		std::cout << "socket : " << server_fd << std::endl;

		// Création de l'adresse de connexion
		struct sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;

		std::cout << "---------------------------------------" << std::endl;
		std::cout << "addr : " << &addr << std::endl;
		std::cout << "sin_family : " << addr.sin_family << std::endl;
		std::cout << "sin_port : " << addr.sin_port  << std::endl;
		std::cout << "sin_addr.s_addr : " << addr.sin_addr.s_addr << std::endl;
		std::cout << "---------------------------------------" << std::endl;



		// Association de l'adresse de connexion au socket
		if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
			throw std::string("Error : Error during the association of socket !\n");

		// Mise en écoute du socket
		if (listen(server_fd, SOMAXCONN) == -1)
			throw std::string("Error : Error during the listing of socket !\n");

		std::cout << "Serveur en attente de connexions sur le port " << port << std::endl;




		int max_clients = 30;
		int client_socket[30];
		int valread;
		int sd;
		int max_sd;
		int addrlen = sizeof(addr);
		// Initialise all client_socket[] to 0
		for (int i = 0; i < max_clients; i++)
		{
			client_socket[i] = 0;
		}

		// // Initialize the set of active sockets
		// fd_set active_sockets;
		// FD_ZERO(&active_sockets);
		// FD_SET(server_fd, &active_sockets);
		// int max_socket_fd = server_fd;

		// // initialisation des ensembles de sockets à surveiller
		// fd_set read_sockets = active_sockets;
		// for (int i = 0 ; i < max_clients ; i++)
		// {
		// 	server_fd = client_socket[i];
		// 	if (server_fd > 0)
		// 		FD_SET(server_fd, &active_sockets);
		// 	if (server_fd > max_socket_fd)
		// 		max_socket_fd = server_fd;
		// }

		// Initialize the read sockets set
		fd_set read_sockets;
		FD_ZERO(&read_sockets);
		FD_SET(server_fd, &read_sockets);
		int max_socket_fd = server_fd;

		fd_set temp;
		// Boucle principale du serveur
		while (true)
		{

			std::cout << "en attende de donnees \n";

			// Wait for any of the sockets to become readable
			temp = read_sockets;
			if (select(max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
				throw std::string("Error : waiting for sockets to become readable !\n");

			// std::cout << "HEREEEEEEEEEEE1\n";
			(void) read_sockets;

			// traitement de l'activité sur les sockets
			if (FD_ISSET(server_fd, &temp))
			{

				// nouvelle connexion entrante
				struct sockaddr_in client_address;
				int addrlen = sizeof(client_address);
				int new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
				if (new_socket < 0)
				{
					perror("Failed to accept connection");
					exit(1);
				}
				FD_SET(new_socket, &temp);
				if (new_socket > max_socket_fd) {
					max_socket_fd = new_socket;
				}
				std::cout	<< "Nouvelle connexion : socket_fd : "
							<< new_socket << "/ ip : " << inet_ntoa(addr.sin_addr)
							<< " / port : " << ntohs(addr.sin_port)
							<< std::endl;
				for (int i = 0; i < max_clients; i++)
				{
					if (client_socket[i] == 0)
					{
						client_socket[i] = new_socket;
						break;
					}
				}
			}

			// Check for data on any of the client sockets
			for (int i = 0; i < max_clients && client_socket[i] != 0; i++)
			{
				std::cout << "client socket " << i << " : " << client_socket[i] << std::endl;

				// std::cout << "heeeerreeee\n";
				int client_socket_fd = client_socket[i];

				if (FD_ISSET(client_socket_fd, &temp))
				{
					std::cout << "heeeerreeee1\n";

					valread = read(client_socket_fd, buffer, 1024);
					if (valread == 0)
					{
						// Client disconnected, remove from active socket set
						close(client_socket_fd);
						FD_CLR(client_socket_fd, &temp);
						client_socket[i] = 0;
					}
					else
					{
						// Echo back the received data
						buffer[valread] = '\0';
						std::cout << "Received message: " << buffer << "\n";
						// send(client_socket_fd, buffer, strlen(buffer), 0);
					}
				}
			}
		}


	(void)sd;
	(void)max_sd;
	(void)client_socket;
	(void)valread;
	(void)addrlen;
	(void)buffer;

		std::cout << "No problems" << std::endl;
	}
	catch(const std::string except)
	{
		if (server_fd)
			close(server_fd);
		std::cout << except;
	}
	return 0;
}
