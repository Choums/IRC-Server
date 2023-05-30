/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:17:20 by marvin            #+#    #+#             */
/*   Updated: 2023/05/08 17:17:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

//	Command: TOPIC
//	Parameters: <channel> [ <topic> ]
//	-----
//	The TOPIC command is used to change or view the topic of a channel.
//	The topic for channel <channel> is returned if there is no <topic>given.
//	If the <topic> parameter is present, the topic for that
//	 channel will be changed, if this action is allowed for the user
//	 requesting it.
//	If the <topic> parameter is an empty string, the
//	 topic for that channel will be removed.
//	-----
//	ERR_NEEDMOREPARAMS		ERR_NOTONCHANNEL
//	RPL_NOTOPIC				RPL_TOPIC
//	ERR_CHANOPRIVSNEEDED	ERR_NOCHANMODES
//	----
//	TOPIC #test :                   ; Command to clear the topic on #test.
//	TOPIC #test                     ; Command to check the topic for #test.
void	Server::cmd_Topic(User& user, std::string const& rest)
{
	std::string	str;				//	Reply
	std::stringstream	ss(rest);	
	std::string			chan;
	std::string			new_topic;	// Topic a changer
	ss >> chan;

	std::cout << YELLOW << "-Topic Command-" << END << std::endl;
	std::cout << RED << "<" << chan << ">" << END << std::endl;
	if (chan.size() == 1)
	{
		str = ERR_NEEDMOREPARAMS(user, "TOPIC");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	Chan_iter	it = this->get_Channel(chan);
	if (it != this->_channel.end())
	{
		Channel *channel = (*it);
		if (std::getline(ss, new_topic, ':'))	// Si getline trouve le ':' => new topic
		{
			if ((channel->Is_TopicLock() && channel->Is_Ope(user)) || !(channel->Is_TopicLock()))
			{
				std::getline(ss, new_topic);	// Extrait le reste du new topic

				RmNewLine(new_topic, '\n');
				RmNewLine(new_topic, '\r');
				std::cout << GREEN << "<" << new_topic << ">, size: " << new_topic.size() << END << std::endl;
				
				// std::cout << "|" << new_topic[1] << "|" << std::endl;
				channel->setTopic(new_topic);
				channel->Broadcast_topic();
				return ;
			}
			else
			{
				str = ERR_CHANOPRIVSNEEDED(user, channel->getName());
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
		}
		else	// Sinon afficher le topic du channel, RPL_TOPIC
		{
			std::cout << YELLOW << "-Display Topic-" << END << std::endl;
			std::string	topic = channel->getTopic();
			if (topic.empty())
				str = RPL_NOTOPIC(user, channel->getName());
			else
				str = RPL_TOPIC(user, channel->getName(), topic);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		}
		
	}
	else
	{
		str = ERR_NOSUCHCHANNEL(user, chan);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

}