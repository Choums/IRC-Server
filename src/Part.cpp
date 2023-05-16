/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:36:42 by marvin            #+#    #+#             */
/*   Updated: 2023/05/02 18:36:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

//	ERR_NEEDMOREPARAMS
//	ERR_NOSUCHCHANNEL
//	ERR_NOTONCHANNEL
// :<user> PART <channel> :<reason>
// :John!~user@host PART #channel :Je reviendrai plus tard

void	Server::cmd_Part(User& user, std::string const& rest)
{
	std::stringstream	ss(rest);
	std::string	chans;
	std::string	reason;
	ss >> chans;
	std::getline(ss, reason, ':');	// Positionne sur le ':'
	std::getline(ss, reason);		// Extrait le msg a partir du ':'

	RmNewLine(reason, '\r');
	RmNewLine(reason, '\n');

	std::vector<std::string> cnl = parse_cnl_name(chans);
	
	std::cout << YELLOW << "-Part Commande-" << END << std::endl;

	std::string	str;
	if (cnl.size() == 0)
	{
		std::cout << RED << "PART -No params given-" << END << std::endl;
		str = ERR_NEEDMOREPARAMS(user, "PART");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}
	if (reason.size() == 1)
	{
		std::cout << "reason empty, default used\n";
		reason = "Bye Bye"; // Reason par defaut si l'user n'en fournit pas
	}
	std::cout << RED << "reason: |" << reason << "|" << END << std::endl;

	for (size_t	i(0); i < cnl.size(); i++)
	{
		std::cout << RED << "Channel: <" << cnl[i] << ">" << END << std::endl; 
		
		Chan_iter	it = this->get_Channel(cnl[i]);

		if (it != this->_channel.end())
		{
			Channel *tmp = *it;
			std::cout << GREEN << "Found Channel: <" << tmp->getName() << ">" << END << std::endl; 
			if (tmp->Is_Present(user.getNickname()))
			{
				tmp->PartUser(user, reason);
				user.setRmCnlMembership(tmp);
				std::cout << RED << "nbr users: " << tmp->getNumUsers() << END << std::endl;
				if (!tmp->getNumUsers() || !tmp->Is_OpePresent()) // Si l'user etait le dernier du Canal ou si l'user etait le dernier oper, le Canal est fermé
					this->setRmChannel(tmp);
			}
			else
			{
				str = ERR_NOTONCHANNEL(user, cnl[i]);
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);		
			}
		}
		else
		{
			str = ERR_NOSUCHCHANNEL(user, cnl[i]);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		}
	}
}