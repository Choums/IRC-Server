/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 18:41:20 by root              #+#    #+#             */
/*   Updated: 2023/06/01 15:44:16 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


/*
 *	Command LIST
 *	Permet d'afficher tout les channels existant sur le serveur
 *	Concatene tout les noms de channel avec un nl en tant que separateur
 *	La list des channels est send a l'user
 *
 * The list command is used to list channels and their topics.  If the
 * <channel> parameter is used, only the status of that channel is
 * displayed.
 * 322 MonPseudo #channel 10 :Discussion générale
 * RPL_LIST
 * RPL_LISTEND
 * Si le Canal demandé n'existe pas, rien n'est renvoyé. Seul les Canaux existant seront renvoyer via RPL_LIST
*/
void	Server::cmd_List(User& user, std::string const& rest)
{
	std::cout << YELLOW << "-LIST Command-" << END << std::endl;
	std::string	str;

	std::cout << RED << " |" << rest << "|, " << rest.size() << END << std::endl;
	if (rest.size() == 1) // Display tout les Canaux du serveur
	{
		std::cout << GREEN << "[Display All Channels]" << END << std::endl;
		for (size_t i(0); i < this->_channel.size(); i++)
		{
			str = RPL_LIST(user, this->_channel[i]);	
			std::cerr << RED << str << END << std::endl;
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		}	
	}
	else // Display uniquement les Canaux demandés et existant
	{
		std::cout << GREEN << "[Display Asked Channels]" << END << std::endl;
		std::vector<std::string>	list_channels =	parse_cnl_name(rest);
		
		for (size_t i(0); i < list_channels.size(); i++)
		{
			Chan_iter	tmp = this->get_Channel(list_channels[i]);
			if (tmp != this->_channel.end())
			{
				str = RPL_LIST(user, (*tmp));
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			}
		}
	}
	str = RPL_LISTEND(user);	
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}
