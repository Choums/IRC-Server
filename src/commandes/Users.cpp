/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 17:04:55 by root              #+#    #+#             */
/*   Updated: 2023/05/05 15:04:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// 	Command: USER
//		Parameters: <username>	<hostname>	<servername>	<realname>
// 			  USER	guest		tolmoon		tolsun			:Ronnie Reagan
//	-----
// 	The USER message is used at the beginning of connection to specify
//    the username, hostname, servername and realname of s new user.
// 
//	It must be noted that realname parameter must be the last parameter,
//   because it may contain space characters and must be prefixed with a
//    colon (':') to make sure this is recognised as such.
// -----
// ERR_NEEDMOREPARAMS
void	Server::cmd_User(User& user, std::string const& rest)
{
	std::string str;
	
	std::cout << YELLOW << "-User Command-" << END << std::endl;
	if (rest.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "USER");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}
	std::istringstream	iss(rest);
	std::string		word;

	if (iss >> word) // First word => Username
	{
		user.setUsername(word);
		std::cout << "Username: <" << word << ">" << std::endl;
	}
	else
	{
		str = ERR_NEEDMOREPARAMS(user, "USER");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}
	
	if (iss >> word) // Second word => Hostname
	{
		user.setHostname(word);
		std::cout << "Hostname: <" << word << ">" << std::endl;
	}
	else
	{
		str = ERR_NEEDMOREPARAMS(user, "USER");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	std::cout << "END USER CMD" << std::endl;

	if (user.is_set() && !user.getSet())
	{
		std::cout << GREEN << "\t--- WELCOME ---\t" << END << std::endl;
		user.setSetUser();
		welcome(user);
	}
		
}
