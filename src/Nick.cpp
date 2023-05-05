/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 12:43:38 by root              #+#    #+#             */
/*   Updated: 2023/05/05 15:11:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/Server.hpp"


//	Verifie si le nickname est deja utilise par un autre user dans le server
bool	Server::is_Used(std::string const& nick)
{
	for (std::vector<User>::iterator it = this->_client_socket_v.begin(); it != this->_client_socket_v.end(); it++)
		if (it->getNickname() == nick)
			return (true);
	return (false);
}


//	Verifie si le nickname est conforme, Pas plus de 9 caracts et pas de caracts speciaux
bool	Server::is_valid(std::string const& nick)
{
	if (nick.size() > 9)
		return (false);
	for (size_t i(0); i < nick.size() - 1; i++)
		if (!std::isalnum(nick[i]))
			return (false);
	return (true);
}

void Server::cmd_Nick(User& user, std::string const& nickname)
{
	std::string str;
	std::stringstream	ss(nickname);
	std::string	nick;
	ss >> nick;
	
	std::cout << YELLOW << "-Nick Command-" << END << std::endl;
	std::cout << RED << "<" << nick << ">" << END << std::endl;
	if (nick.size() == 1) // ERR_NEEDPARAMS
	{
		str = ERR_NEEDMOREPARAMS(user, "NICK");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	if (this->is_Used(nick)) // ERR_NICKNAMEINUSE
	{
		str = ERR_NICKNAMEINUSE(user, nick);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	}
	else if (!this->is_valid(nick)) // ERR_ERRONEUSNICKNAME
	{
		str = ERR_ERRONEUSNICKNAME(user);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	}
	else
	{
		std::cout << GREEN << "Nickname has been changed to: [" << nick << "]" << END << std::endl;
		str = NICK(user, nick);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		// std::cout << "||| " << str << " |||" << std::endl;
		user.setNickname(nick);

		if (user.is_set() && !user.getSet())
		{
			user.setSetUser();
			welcome(user);
		}
	}
	
}
