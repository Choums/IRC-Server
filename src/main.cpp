/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:00 by aptive            #+#    #+#             */
/*   Updated: 2023/04/13 15:40:33 by aptive           ###   ########.fr       */
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




	try
	{
		// Parsing arguments ---------------------------------------------------------------
		parsing(argc, argv, &port, &password);

		// Configuration socket et adresse server ------------------------------------------
		ft_socket_addr_server(&server_fd, &addr, port);

		std::cout << "socket : " << server_fd << std::endl;
		std::cout << "Serveur en attente de connexions sur le port " << port << std::endl;


		boucle_server(server_fd, addr);


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
