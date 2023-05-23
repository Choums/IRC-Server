/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   affichage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelauna <tdelauna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:23:07 by aptive            #+#    #+#             */
/*   Updated: 2023/05/23 17:59:41 by tdelauna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/User.hpp"

void affichage_list(std::map<std::string, std::string> list)
{
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = list.begin(); it != list.end(); it++)
	{
		i++;
		std::cout << "list " << i << " < " << it->first << ", " << it->second << " >"<< std::endl;
	}
}

void affichage_socket(int client_server_fd, struct sockaddr_in client_addr)
{
	std::cout << "---------------------------------------" << std::endl;
	std::cout << "Nouvelle connexion entrante" << std::endl;
	std::cout << "client_socket fd : " << client_server_fd << std::endl;
	std::cout << "addr : " << &client_addr << std::endl;
	std::cout << "sin_family : " << client_addr.sin_family << std::endl;
	std::cout << "sin_port : " << client_addr.sin_port  << std::endl;
	std::cout << "sin_addr.s_addr : " << client_addr.sin_addr.s_addr << std::endl;
	std::cout << "---------------------------------------" << std::endl;
}
