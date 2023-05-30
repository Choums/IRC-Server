/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/05/30 15:28:36 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/User.hpp"
#include "../includes/Server.hpp"

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
	if (user.getAuth_password() == true)
	{
		std::string    str = RPL_WELCOME(user) + RPL_YOURHOST(user) + RPL_CREATED(user) + RPL_MYINFO(user);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	}
	else
	{
		Server::running_serv->setRmUser(user);
	}
}

// Ctrl+a, n : Cette combinaison de touches vous permet de passer à la fenêtre de discussion suivante.

// Ctrl+a, p : Cette combinaison de touches vous permet de passer à la fenêtre de discussion précédente.

// Ctrl+a, 1-9 : Ces combinaisons de touches vous permettent de basculer directement vers une fenêtre de discussion spécifique.
// Par exemple, "Ctrl+a, 3" vous amènera à la fenêtre de discussion numéro 3.



void	sig_int(int code)
{
	(void)code;
	g_signal = false;
}

Server* Server::running_serv = NULL;

int main(int argc, char ** argv)
{
	int port;
	std::string password;

	signal(SIGINT, &sig_int);
	g_signal = true;

	std::cout << GREEN <<"---------------- SERVER IRC -------------------------------" << END << std::endl;

	try
	{
		// Parsing arguments ---------------------------------------------------------------
		parsing(argc, argv, &port, &password);

		Server server(port, password);
		std::cout << server;
		Server::running_serv = &server;
		// Configuration socket et adresse server ------------------------------------------
		server.boucle_server();
		close(port);
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
