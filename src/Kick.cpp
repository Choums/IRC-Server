/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 15:15:30 by marvin            #+#    #+#             */
/*   Updated: 2023/05/07 15:15:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

//	/Kick <channel> <nickname> <reason>
// 	-----
// The KICK command can be used to request the forced removal of a user
//    from a channel.  It causes the <user> to PART from the <channel> by force.
// If a "comment" is given, this will be sent instead of the default message,
//	the nickname of the user issuing the KICK.
//	-----
// 5-ERR_USERNOTINCHANNEL	1-ERR_NOSUCHCHANNEL	3-ERR_CHANOPRIVSNEEDED	2-ERR_NOTONCHANNEL	4-ERR_NOSUCHNICK
void	Server::cmd_Kick(User& user, std::string const& rest)
{
	std::string			str;
	std::stringstream	ss(rest);
	std::string			chan;
	std::string			target;
	std::string			reason;
	ss >> chan >> target;
	std::getline(ss, reason, ':');

	std::cout << YELLOW << "-Kick Command-" << END << std::endl;
	std::cout << RED << "<" << chan << ">\n<" << target << ">" << ">\n<" << reason << ">" << END << std::endl;

	if (chan.size() == 1 || target.size() == 1)
	{
		str = ERR_NEEDMOREPARAMS(user, "KICK");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	std::cout << RED << reason.size() << "|" << reason << "|" << END << std::endl;
	if (reason.size() == 1)
	{
		std::cout << "reason empty, default used\n";
		reason = ":So long"; // Reason par defaut si l'user n'en fournit pas
	}
	RmNewLine(reason, '\r');
	RmNewLine(reason, '\n');

	Chan_iter	it = this->get_Channel(chan);
	if (it != this->_channel.end())
	{
		Channel	*channel = (*it);
		if (channel->Is_Present(user.getNickname()) || this->is_Ope(user)) // Kicker present dans le channel ou Serv Oper
		{
			if (channel->Is_Ope(user) || this->is_Ope(user)) //	Kicker est operateur du channel ou serv Oper
			{
				User_iter	itu = this->get_User(target);
				if (itu != this->_client_socket_v.end())
				{
					User *kuser = (*itu);
					if (channel->Is_Present(kuser->getNickname()))
					{
						channel->KickUser(user, *kuser, reason);
						if (!channel->Is_OpePresent())
							this->setRmChannel(channel);
					}
					else
					{
						str = ERR_NOUSERONCHANNEL(user, channel->getName(), target);
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
				str = ERR_CHANOPRIVSNEEDED(user, channel->getName());
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
		}
		else
		{
			str = ERR_NOTONCHANNEL(user, channel->getName());
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
