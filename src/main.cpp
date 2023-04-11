/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/11 16:50:29 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

int main(int argc, char ** argv)
{
	// std::string port;
	std::string password;

	int port;
	int sockfd(-1);
	try
	{
		check_arg(argc, argv);

		port = atoi(argv[1]);
		password = argv[2];

		std::cout << "Port / Password : " << port << " / " << password << std::endl;

		// Creation socket

		int sockfd = socket (AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
			throw std::string("Error : Error during the creation of socket !\n");



		// configuration du socket
		int yes = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
			throw std::string("Error : Error during the configuration of socket !\n");


		std::cout << "socket : " << sockfd << std::endl;

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
		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
			throw std::string("Error : Error during the association of socket !\n");

		// Mise en écoute du socket
		if (listen(sockfd, SOMAXCONN) == -1)
			throw std::string("Error : Error during the listing of socket !\n");

		std::cout << "Serveur en attente de connexions sur le port " << port << std::endl;

		// Boucle principale du serveur
		while (true)
		{
			// Accepter une nouvelle connexion entrante
			struct sockaddr_in client_addr;
			socklen_t client_addr_len = sizeof(client_addr);
			int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
			if (client_sockfd == -1)
			{
				std::cerr << "Erreur lors de l'acceptation d'une connexion entrante" << std::endl;
				continue;
			}


			// TODO: traiter la connexion entrante
			std::cout << "---------------------------------------" << std::endl;
			std::cout << "Nouvelle connexion entrante" << std::endl;
			std::cout << "client_socket fd : " << client_sockfd << std::endl;
			std::cout << "addr : " << &client_addr << std::endl;
			std::cout << "sin_family : " << client_addr.sin_family << std::endl;
			std::cout << "sin_port : " << client_addr.sin_port  << std::endl;
			std::cout << "sin_addr.s_addr : " << client_addr.sin_addr.s_addr << std::endl;
			std::cout << "---------------------------------------" << std::endl;

			// receive a message
			int ok = 1;
			while (ok)
			{
				char buffer[1024];
				ssize_t numBytes = recv(client_sockfd, buffer, sizeof(buffer), 0);
				std::cout << "heere\n";
				if (numBytes == -1) {
					std::cerr << "Error receiving message\n";
					// handle error
				}
				else if (numBytes == 0) {
					std::cout << "Connection closed by peer\n";
					ok = 0;
					// handle connection closed by peer
				}
				else {
					buffer[numBytes] = '\0'; // add null terminator
					std::cout << "Received message: " << buffer << "\n";
					// handle received message
				}
			}
			// Fermer le socket de la connexion
			// close(client_sockfd);
		}

	// Fermer le socket d'écoute
		close(sockfd);

		std::cout << "No problems" << std::endl;
	}
	catch(const std::string except)
	{
		if (sockfd)
			close(sockfd);
		std::cout << except;
	}
	return 0;
}
