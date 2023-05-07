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
		if (!user.compare((*it)->getNickname()))
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
	std::cout << YELLOW << "-Invite Command-" << END << std::endl;
	std::cout << RED << "Tyring to invite " << target << " to " << chan << END << std::endl;
	if (!rest.empty() || !target.empty() || !chan.empty()) // Check Params
	{
		Chan_iter it = this->get_Channel(chan);
		if (it != this->_channel.end()) // Check Channel exists, NOSUCHCHANNEL 
		{
			if ((*it)->Is_Present(user.getNickname())) // Check User is on Channel, NOTONCHANNEL
			{
				if ((*it)->Is_Ope(user)) // Check User has priv, CHANOPRIVSNEEDED
				{
					if (this->is_Present(target)) // Check target exists, NOSUCHNICK
					{
						if (!(*it)->Is_Ban(*(*(this->get_User(target))))) // Check target isn't banned from Channel
						{
							if (!(*it)->Is_Present(target)) // Check target not already in Channel, USERONCHANNEL
							{
								std::cout << GREEN << "All condition are passed, adding " << target << " to " << chan << END << std::endl;
								(*it)->InvUser(user, *(*(this->get_User(target)))); // Invitation complete !
							}
							else
							{
								str = ERR_USERONCHANNEL(user, target, chan);
								send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
								return ;
							}
						}
						else
						{
							str = ERR_BANNEDFROMCHAN(user, (*it));
							send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
							return ;
						}
					}
					else
					{
						str = ERR_NOSUCHNICK(user, target);
						std::cout << RED << "No such nick <" << target << ">" << END << std::endl;
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
