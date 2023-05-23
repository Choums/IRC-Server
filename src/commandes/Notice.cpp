/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 18:01:33 by root              #+#    #+#             */
/*   Updated: 2023/05/17 18:32:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::cmd_Notice(User& user, std::string const& rest)
{
	std::string			str;
	
	std::stringstream	ss(rest);
	std::string			target;
	ss >> target;
	
	std::string			msg;
	std::getline(ss, msg, ':');
	std::getline(ss, msg);

	if (target.size() == 1)
	{
		str = ERR_NEEDMOREPARAMS(user, "NOTICE");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	if (msg.size() == 1)
	{
		str = ERR_NOTEXTTOSEND(user);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	RmNewLine(msg, '\n');
	RmNewLine(msg, '\r');

	if (target[0] == '#') // Channel Broadcast
	{
		Chan_iter	it = this->get_Channel(target);
		if (it != this->_channel.end())
		{
			Channel	*channel = *it;
			if (!channel->Is_Ban(user))
			{
				std::string	notice = ":" + user.getNickname() + " NOTICE " + channel->getName() + " :" + msg + "\r\n";
				channel->Broadcast(notice);
			}
		}
	}
	else // User Notice
	{
		User_iter	itu = this->get_User(target);
		if (itu != this->_client_socket_v.end())
		{
			User	*receiver = *itu;
			std::string	notice = ":" + user.getNickname() + " NOTICE " + receiver->getNickname() + " :" + msg + "\r\n";
			send(receiver->getFd(), notice.c_str(), notice.size(), MSG_NOSIGNAL);
		}
	}
}
