/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:16:54 by root              #+#    #+#             */
/*   Updated: 2023/04/19 17:49:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

/*
 *	Veille a ce que le nom du canal corresponde a la norme RFC 2812
 *	-> beginning with a '&'
 *	-> length up to fifty (50) characters.
 *	-> SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (',')
 *	-> channel names SHOULD NOT be allowed to be reused
 *	->
*/
std::vector<std::string>	parse_cnl_name(std::string const& rest)
{
	if (rest.find(" ") != std::string::npos)
		throw std::string("Invalid argument format !");

	std::vector<std::string>	cnl;

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
	return (cnl);
}