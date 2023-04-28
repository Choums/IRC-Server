/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/27 18:15:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/User.hpp"
#include "../includes/Server.hpp"

void sendMessageSuccess(int fd, std::string message)
{
	std::string send_msg = GREEN + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), MSG_NOSIGNAL);
}

void sendMessageUnSuccess(int fd, std::string message)
{
	std::string send_msg = RED + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), MSG_NOSIGNAL);
}

void sendMessageWarning(int fd, std::string message)
{
	std::string send_msg = YELLOW + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), MSG_NOSIGNAL);
}


void sendMessageSucces(int fd, std::string message)
{
	std::string send_msg = GREEN + message + END;
	send(fd, send_msg.c_str(), send_msg.size(), MSG_NOSIGNAL);
}

void sendMessage(int fd, std::string message)
{
	send(fd, message.c_str(), message.size(), MSG_NOSIGNAL);
}

// RPL_WELCOME (001) : Ce code est envoyé en premier pour accueillir le client sur le serveur et pour lui indiquer que la connexion a réussi.
// 001 "Welcome to the Internet Relay Network <nick>!<user>@<host>"
// RPL_YOURHOST (002) : Ce code est utilisé pour indiquer au client le nom et la version du serveur IRC auquel il est connecté.
// 002 "Your host is <servername>, running version <ver>"
// RPL_CREATED (003) : Ce code est envoyé pour indiquer au client la date de création du serveur IRC.
// 003 "This server was created <date>"
// RPL_MYINFO (004) : Ce code fournit au client des informations sur le serveur IRC, y compris son nom, sa version, les modes pris en charge et les extensions disponibles.
// 004 "<servername> <version> <available user modes> <available channel modes>"
void	welcome(User& user)
{
	std::string	str = RPL_WELCOME(user) + RPL_YOURHOST(user) + RPL_CREATED(user) + RPL_MYINFO(user);

	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

Server* Server::running_serv = NULL;

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
		Server::running_serv = &server;
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
