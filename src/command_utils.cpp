/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:16:54 by root              #+#    #+#             */
/*   Updated: 2023/05/03 16:31:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

/*
 *	Permet de gerer les arguments de noms de channels: #sad ou #sad,#asd
 *	Veille a ce que le nom du canal corresponde a la norme RFC 2812
 *	-> beginning with a '&'
 *	-> length up to fifty (50) characters.
 *	-> SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (',')
 *	-> channel names SHOULD NOT be allowed to be reused
 *	
*/
std::vector<std::string>	parse_cnl_name(std::string const& line)
{
	std::cout << RED << "---parse---" << END << std::endl;
	std::vector<std::string>	cnl;
	std::stringstream			ss(line);
	std::string					rest;
	
	ss >> rest;
	std::cout << rest << std::endl;
	if (rest.empty())
	{
		std::cout << "REST EMPTY" << std::endl;
		std::cout << RED << "---end parse---" << END << std::endl;
		return (cnl);
	}
	// RECUPERATION DES NOMS DE CANALS
	
	if (rest.find(",") == std::string::npos)
		cnl.push_back(rest);
	else
	{
		size_t	pos(0);
		size_t	new_pos(0);
		
		while (new_pos != std::string::npos)
		{
			new_pos = rest.find(",", pos);
			std::cout << RED << "==> " << rest.substr(pos, new_pos) << END << std::endl;
			cnl.push_back(rest.substr(pos, new_pos));
			pos = new_pos + 1;
		}
	}
	std::cout << RED << "---end parse---" << END << std::endl;
	return (cnl);
}