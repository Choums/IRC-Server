/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:19:37 by root              #+#    #+#             */
/*   Updated: 2023/04/26 16:43:51 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// Check le server
void	Server::cmd_Ping(User& user, std::string const& target)
{
	std::string str;
	if (target.empty())
	{
		str = ERR_NOORIGIN();
		send(user.getFd(), str.c_str(), str.size(), 0);
		return ;
	}
	else
	{
		str = PONG(user);
		send(user.getFd(), str.c_str(), str.size(), 0);
	}
}