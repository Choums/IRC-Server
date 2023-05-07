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
 * 	/join &foobar		|	Command to join channel &foobar.
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

	std::vector<std::string> cnl = parse_cnl_name(rest);

	if (cnl.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "JOIN");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	for (size_t	i(0); i < cnl.size(); i++)
	{
		if (channel_exist(cnl[i])) // Verif que le channel existe dans le serveur
		{
			Channel *tmp = *(this->get_Channel(cnl[i]));
			if (tmp->Is_Ban(user))
			{
				str = ERR_BANNEDFROMCHAN(user, tmp);
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
			
			if (tmp->Is_InvOnly() && !(tmp->Is_Inv(user))) // le Canal est +i et l'user n'y est pas invité
			{
				
				str = ERR_NEEDINVITE(user, tmp->getName());
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}

			// for (size_t i(0); i < this->_channel.size(); i++)
			// 	if (!cnl[i].compare(this->_channel[i]->getName()))
			// 		this->_channel[i]->AddUser(user, false);
			tmp->AddUser(user, false);
			user.setAddListCnlMember(this->_channel[i]);
			// this->_channel[i].getUsers();
		}
		else // Le channel est cree
		{
			std::cout << RED << "==>>>> |" << cnl[i] << "| <<<<==" << END << std::endl;
			Channel *chan = new Channel(user, cnl[i]);
			this->setNewChannel(chan);
			user.setAddListCnlMember(chan);
		}

	}
}