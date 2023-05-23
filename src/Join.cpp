/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 17:58:26 by marvin            #+#    #+#             */
/*   Updated: 2023/04/27 17:58:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/*
 *	Command: JOIN
 *	Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
 *            / "0"
 *	----------
 *	The JOIN command is used by a user to request to start listening to
 *		the specific channel.
 *
 *	Once a user has joined a channel, he receives information about
 *		all commands his server receives affecting the channel.
 *
 *	If a JOIN is successful, the user receives a JOIN message
 *		as confirmation and is then sent the channel's topic
 *			and the list of users who are on the channel,
 *				which MUST include the user joining.
 *
 *	Note that this message accepts a special argument ("0"), which is
 *		a special request to leave all channels the user is currently a member of.
 *
 * 	----------
 * @example
 * 	/join &foobar key	|	Command to join channel &foobar with a pass.
 *	/join &foo,&bar		|	Command to join channels &foo and &bar.
 *	/join 0				|	Leave all currently joined channels.
 *
 *	----------
 *	Si le rest est "0", l'User quitte tout les channels dont il fait parti
 *	Le rest est une premiere fois parser, renvoie un vector avec le ou les channels
 *	Si le nom du Canal n'est pas connu, il est cree
 *	Sinon l'User est ajoute au Canal
 *
 * ERR_NOSUCHCHANNEL	ERR_BANNEDFROMCHAN	
*/
void	Server::cmd_JoinChannel(std::string const& rest, User& user)
{
	// std::cout << RED << "rest: |" << rest << "|, " << rest.size() << END << std::endl;
	std::string	str;
	if (rest.size() == 2 && rest[0] == '0')
	{
		std::cout <<RED << "[SERVER] : [" << user.getNickname() << "] leaving all channels" << END << std::endl;
		
		std::vector<Channel *>	list_cnls = user.getListCnl();
		std::string	format;
		for (size_t i(0); i < list_cnls.size(); i++)
		{
			format.append(list_cnls[i]->getName());
			if (i + 1 != list_cnls.size())
				format.push_back(',');
		}
		this->cmd_Part(user, format);
		return ;
	}

	std::map<std::string, std::string> cnl = parse_cnl_name_with_key(rest);

	if (cnl.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "JOIN");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	std::map<std::string, std::string>::iterator	it = cnl.begin();
	std::map<std::string, std::string>::iterator	ite = cnl.end();

	while (it != ite)
	{
		std::cout << RED << "chan: " << it->first << " | key: " << it->second << END << std::endl;

		if (channel_exist(it->first)) // Verif que le channel existe dans le serveur
		{
			Channel *channel = *this->get_Channel(it->first);
			if (!channel->Is_Ban(user))
			{
				if (channel->Is_InvOnly() && channel->Is_Inv(user))
				{
					str = ERR_NEEDINVITE(user, channel->getName());
					send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
					return ;
				}

				if (channel->getPass() == it->second)
				{
					std::cout << RED << "Limit[" << channel->Is_limitSet() << "]: " << channel->getCapacity() << " | " << channel->getNumUsers() << END << std::endl;
					if (channel->Is_limitSet() && channel->getNumUsers() >= channel->getCapacity())
					{
						str = ERR_CHANNELISFULL(user, channel->getName());
						send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
						return ;
					}

					channel->AddUser(user, false);
					user.setAddListCnlMember(channel);
				}
				else
				{
					str = ERR_BADCHANNELKEY(user, channel->getName());
					send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
					return ;
				}
			}
			else
			{
				str = ERR_BANNEDFROMCHAN(user, channel);
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
		}
		else	// Le channel est cree
		{
			Channel *chan = new Channel(user, it->first, it->second);
			this->setNewChannel(chan);
			user.setAddListCnlMember(chan);
		}
		it++;
	}
}