/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/13 15:21:32 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/User.hpp"

int main(int argc, char ** argv)
{
	int port;
	std::string password;

	int server_fd(-1);
	struct sockaddr_in addr;


	char buffer[1025];


	try
	{
		// Parsing arguments ------------------------------------------
		check_arg(argc, argv);
		port = atoi(argv[1]);
		password = argv[2];
		std::cout << "Port / Password : " << port << " / " << password << std::endl;


		ft_socket_addr_server(&server_fd, &addr, port);

		std::cout << "socket : " << server_fd << std::endl;
		std::cout << "Serveur en attente de connexions sur le port " << port << std::endl;



		std::vector<User> client_socket_v;
		int valread;



		// Initialize the read sockets set
		fd_set read_sockets;
		FD_ZERO(&read_sockets);
		FD_SET(server_fd, &read_sockets);
		int max_socket_fd = server_fd;

		fd_set temp;
		// Boucle principale du serveur
		int count(0);
		while (true)
		{
			std::cout << "en attende de donnees : " << YELLOW << count << END << std::endl;
			count++;
			if (count > 20)
				return 0;


			// Wait for any of the sockets to become readable
			temp = read_sockets;
			if (select(max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
				throw std::string("Error : waiting for sockets to become readable !\n");

			(void) read_sockets;

			// traitement de l'activité sur les sockets
			if (FD_ISSET(server_fd, &temp))
			{
				// nouvelle connexion entrante
				struct sockaddr_in client_address;
				int addrlen = sizeof(client_address);
				int new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);

				if (new_socket < 0)
					throw std::string("Error : Failed to accept connection !\n");

				FD_SET(new_socket, &read_sockets);
				if (new_socket > max_socket_fd)
					max_socket_fd = new_socket;

				std::cout	<< "Nouvelle connexion : socket_fd : "
							<< new_socket << "/ ip : " << inet_ntoa(addr.sin_addr)
							<< " / port : " << ntohs(addr.sin_port)
							<< std::endl;


				client_socket_v.push_back(User(new_socket));
			}

			affichage_vector(client_socket_v);

			// Check for data on any of the client sockets
			for (size_t i = 0; i < client_socket_v.size(); i++)
			{
				int client_socket_fd = client_socket_v[i].getFd();
				if (FD_ISSET(client_socket_fd, &temp))
				{
					valread = read(client_socket_fd, buffer, 1024);
					std::cout << "valread on client socket : " << valread << " / "<< client_socket_fd << std::endl;
					if (valread == 0)
					{
						std::cout << RED << "Delete \n" << client_socket_v[i] << END << std::endl;
						// Client disconnected, remove from active socket set
						close(client_socket_fd);
						FD_CLR(client_socket_fd, &read_sockets);
						client_socket_v.erase(client_socket_v.begin()+i);
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
