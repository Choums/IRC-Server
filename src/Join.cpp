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
*/
void	Server::cmd_JoinChannel(std::string const& rest, User& user)
{
	std::cout << RED << "rest: |" << rest << "|, " << rest.size() << END << std::endl;
	if (rest.size() == 2 && rest[0] == '0')
	{
		std::cout << "[SERVER] : [" << user.getNickname() << "] leaving all channels" << std::endl;
		user.LeaveCnls();
		return ;
	}
	std::vector<std::string> cnl = parse_cnl_name(rest);

	for (size_t	i(0); i < cnl.size(); i++)
	{
		if (channel_exist(cnl[i])) // Verif que le channel existe dans le serveur
		{
			// std::cout << "there\n" ;
			for (size_t i(0); i < this->_channel.size(); i++)
				if (!cnl[i].compare(this->_channel[i].getName()))
					this->_channel[i].AddUser(user, false);
			user.setAddListCnlMember(this->_channel[i]);
			// this->_channel[i].getUsers();
		}
		else // Le channel est cree
		{
			if (cnl[i][0] != '#' || cnl[i].size() > 50)
				break;
				// throw std::string("Invalid channel name !");
			Channel chan;
			chan.setName(rest);
			chan.AddUser(user, true);
			this->setNewChannel(chan);
			user.setAddListCnlMember(chan);
		}

	}
}