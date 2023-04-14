/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_irc.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 16:50:46 by aptive            #+#    #+#             */
/*   Updated: 2023/04/14 17:11:43 by aptive           ###   ########.fr       */
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

// void search_cmd(std::string cmd, User * user)
// {

// 	std::cout << "le user dans le parsing : \n";
// 	// std::cout << user;




// 	(void)cmd;
// 	// user.handleCommand(cmd);
// 	user->setNickname();

// }

void parsing_cmd(std::string buffer, User * user)
{
	std::cout << "Received message: " << buffer << "\n";
	std::string cmd;
	std::string rest;
	std::vector<std::string> v_parse;


	(void)user;
	v_parse = split_string(buffer);

	// Affiche chaque élément du vecteur tokens
	for (std::vector<std::string>::iterator it = v_parse.begin(); it != v_parse.end(); ++it) {
		std::cout << *it << "|" << std::endl;
	}

	if (v_parse[0][0] == '/')
	{
		std::cout << "It's a commande !" << std::endl;
		user->handleCommand(v_parse[0], v_parse[1]);

	}
	// {
	// 	int j;
	// 	for (size_t i = 1; i < buffer.size() && buffer[i] != ' '; i++)
	// 	{
	// 		if (isalpha(buffer[i]))
	// 			cmd += buffer[i];
	// 		j = i;
	// 	}
	// 	rest = buffer.substr(j + 1, buffer.size() - j);

	// 	std::cout << "rest : " << rest << std::endl;
	// 	search_cmd(cmd, user);
	// }


	// for (size_t i = 0; i < buffer.size(); i++)
	// {
	// 	std::cout << buffer[i] << std::endl;
	// }
}
