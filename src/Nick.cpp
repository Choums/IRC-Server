/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 12:43:38 by root              #+#    #+#             */
/*   Updated: 2023/04/26 19:30:21 by root             ###   ########.fr       */
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

void Server::cmd_Nick(User& user, std::string const& nick)
{
	std::string str;
	if (nick.empty()) // ERR_NEEDPARAMS
	{
		str = "err noparams\n";
		send(user.getFd(), str.c_str(), str.size(), 0);
		return ;
	}

	if (this->is_Used(nick)) // ERR_NICKNAMEINUSE
	{
		str = ERR_NICKNAMEINUSE(user, nick);
		send(user.getFd(), str.c_str(), str.size(), 0);
	}
	else if (!this->is_valid(nick)) // ERR_ERRONEUSNICKNAME
	{
		str = ERR_ERRONEUSNICKNAME(user);
		send(user.getFd(), str.c_str(), str.size(), 0);
	}
	// else if ()
	else
	{
		str = NICK(user, nick);
		std::cout << "||| " << str << " |||" << std::endl;
		user.setNickname(nick);
	}
}
