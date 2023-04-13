/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_irc.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:50:46 by aptive            #+#    #+#             */
/*   Updated: 2023/04/13 17:20:19 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

void search_cmd(std::string cmd)
{
	std::string levels[4] = { "nick", "join"};
	std::cout << "entree |" << cmd.size() << "|"<< std::endl;

	for (int i = 0; i < 4; i++)
	{
		if ( !cmd.compare(levels[i]))
			std::cout << "La commande : " << levels[i] << std::endl;
	}
}

void parsing_cmd(std::string buffer)
{
	std::cout << "Received message: " << buffer << "\n";
	std::string cmd;
	if (buffer[0] == '/')
	{
		std::cout << "It's a comamnde !" << std::endl;
		for (size_t i = 1; i < buffer.size() && buffer[i] != ' '; i++)
		{
			if (isalpha(buffer[i]))
				cmd += buffer[i];
		}
		search_cmd(cmd);
	}


	// for (size_t i = 0; i < buffer.size(); i++)
	// {
	// 	std::cout << buffer[i] << std::endl;
	// }
}
