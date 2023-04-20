/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/19 18:38:10 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/User.hpp"
#include "../includes/Server.hpp"

void sendMessageSuccess(int fd, std::string message)
{
	std::string send_msg = GREEN + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), 0);
}

void sendMessageUnSuccess(int fd, std::string message)
{
	std::string send_msg = RED + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), 0);
}

void sendMessageWarning(int fd, std::string message)
{
	std::string send_msg = YELLOW + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), 0);
}


void sendMessageSucces(int fd, std::string message)
{
	std::string send_msg = GREEN + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), 0);
}

void sendMessage(int fd, std::string message)
{
	send(fd, message.c_str(), message.size(), 0);
}

int main(int argc, char ** argv)
{
	int port;
	std::string password;

	// int server_fd(-1);
	// struct sockaddr_in addr;


	std::cout << GREEN <<"---------------- SERVER IRC -------------------------------" << END << std::endl;

	try
	{
		// Parsing arguments ---------------------------------------------------------------
		parsing(argc, argv, &port, &password);

		Server server(port, password);
		std::cout << server;

		// Configuration socket et adresse server ------------------------------------------
		// ft_socket_addr_server(&server_fd, &addr, port);

		// std::cout << "socket : " << server_fd << std::endl;
		// std::cout << "Serveur en attente de connexions sur le port " << port << std::endl;


		// boucle_server(server.getServer_fd(), server.getAddr());
		server.boucle_server();


		std::cout << "No problems" << std::endl;
	}
	catch(const std::string except)
	{
		// if (server_fd)
		// 	close(server_fd);
		std::cout << except;
	}
	return 0;
}
