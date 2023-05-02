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
	
	ss >> chans >> reason;
	std::vector<std::string> cnl = parse_cnl_name(chans);
	
	std::string	str;
	if (cnl.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "PART");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}
	std::cout << RED << reason.size() << "|" << reason << "|" << END << std::endl;
	if (reason.empty())
		reason = "Bye Bye\n"; // Reason par defaut si l'user n'en fournit pas

	for (size_t	i(0); i < cnl.size(); i++)
	{
		Chan_iter	it = this->get_Channel(cnl[i]);
		if (it != this->_channel.end())
		{
			if ((*it)->Is_Present(user.getUsername()))
			{
				(*it)->PartUser(user, reason);
				user.setRmCnlMembership(*it);
				if ((*it)->getNumUsers() == 0) // Si l'user etait le dernier du Canal, le Canal est fermé
					this->setRmChannel(*it);
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