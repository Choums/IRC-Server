/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:09:22 by marvin            #+#    #+#             */
/*   Updated: 2023/04/28 15:09:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


bool	Server::is_Present(std::string const& user)
{
	User_iter	it = this->_client_socket_v.begin();
	User_iter	ite = this->_client_socket_v.end();

	while (it != ite)
	{
		if (!user.compare(it->getNickname()))
			return (true);
		it++;
	}
	return (false);
}

// /invite <User> <#Chan>
//	RPL_INVITING	ERR_NOSUCHNICK	ERR_USERONCHANNEL	ERR_NOTONCHANNEL
//	ERR_CHANOPRIVSNEEDED	ERR_NEEDMOREPARAMS



void	Server::cmd_Invite(User& user, std::string const& rest)
{
	std::istringstream	iss(rest);
	std::string	target;	// User to invite
	std::string	chan;	// Chan to join
	std::string	str;

	iss >> target;
	iss >> chan;

	if (!rest.empty() || !target.empty() || !chan.empty()) // Check Params
	{
		Chan_iter it = this->get_Channel(chan);
		if (it != this->_channel.end()) // Check Channel exists, NOSUCHCHANNEL 
		{
			if (it->Is_Present(user.getNickname())) // Check User is on Channel, NOTONCHANNEL
			{
				if (it->Is_Ope(user)) // Check User has priv, CHANOPRIVSNEEDED
				{
					if (this->is_Present(target)) // Check target exists, NOSUCHNICK
					{
						if (!it->Is_Present(target)) // Check target not already in Channel, USERONCHANNEL
							it->InvUser(user, *(this->get_User(target))); // Invitation complete !
						else
						{
							str = ERR_USERONCHANNEL(user, target, chan);
							send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
							return ;
						}
					}
					else
					{
						str = ERR_NOSUCHNICK(user, target);
						send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
						return ;	
					}
				}
				else
				{
					str = ERR_CHANOPRIVSNEEDED(user, chan);
					send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
					return ;
				}
			}
			else
			{
				str = ERR_NOTONCHANNEL(user, chan);
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
		}
		else
		{
			str = ERR_NOSUCHCHANNEL(user, chan);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			return ;
		}
	}
	else
	{
		str = ERR_NEEDMOREPARAMS(user, "INVITE");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}
}
