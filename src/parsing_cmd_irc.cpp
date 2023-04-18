/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_irc.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:50:46 by aptive            #+#    #+#             */
/*   Updated: 2023/04/18 13:39:17 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"
#include "../includes/User.hpp"

std::vector<std::string> split_string(std::string str)
{
	std::stringstream ss(str);
	std::string token;
	std::vector<std::string> tokens;

	while (std::getline(ss, token, ' ')) {
		tokens.push_back(token);
	}

	return tokens;
}

void parsing_cmd(std::string buffer, User * user)
{
	std::string cmd;
	std::string rest;
	std::vector<std::string> v_parse;
	(void)buffer;


	for (size_t i = 0; i < user->getBuf().size(); i++)
	{
		std::cout << (int)user->getBuf()[i] << std::endl;
	}

	std::cout << "END buffer" << std::endl;

	(void)user;
	v_parse = split_string(user->getBuf());

	// Affiche chaque élément du vecteur tokens
	// for (std::vector<std::string>::iterator it = v_parse.begin(); it != v_parse.end(); ++it) {
	// 	std::cout << *it << "|" << std::endl;
	// }

	if (v_parse[0][0] == '/')
	{
		std::cout << "It's a commande !" << std::endl;
		user->handleCommand(v_parse[0], v_parse[1]);
	}

	user->clearBuf();
}
