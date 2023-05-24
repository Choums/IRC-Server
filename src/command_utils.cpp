/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:16:54 by root              #+#    #+#             */
/*   Updated: 2023/05/24 16:02:30 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

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
	RmNewLine(rest, '\n');
	RmNewLine(rest, '\r');
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

std::map<std::string, std::string>	parse_cnl_name_with_key(std::string const& line)
{
	std::cout << RED << "-parse channel names with key-" << END << std::endl;
	std::map<std::string, std::string>	cnl;
	std::vector<std::string>			cnl_names;
	std::vector<std::string>			cnl_keys;
	std::stringstream					ss(line);
	std::string							rest;
	std::string							keys;

	ss >> rest;
	ss >> keys;
	std::cout << RED << rest << "\n" << keys << std::endl;
	if (rest.empty())
	{
		std::cout << "REST EMPTY" << std::endl;
		std::cout << RED << "---end parse---" << END << std::endl;
		return (cnl);
	}
	// RECUPERATION DES NOMS DE CANALS

	cnl_names = parse_cnl_name(rest);
	cnl_keys = parse_cnl_name(keys);

	for (size_t	i(0); i < cnl_names.size(); i++)
	{
		if (i < cnl_keys.size())
			cnl.insert(make_pair(cnl_names[i], cnl_keys[i]));
		else
			cnl.insert(make_pair(cnl_names[i], std::string("")));
	}


	std::cout << RED << "---end parse---" << END << std::endl;
	return (cnl);
}

bool	str_is_digit(std::string const& str)
{
	for (size_t i(0); i < str.size(); i++)
		if (!std::isdigit(str[i]))
			return (false);
	return (true);
}

bool	str_is_alpha(std::string const& str)
{
	for (size_t i(0); i < str.size(); i++)
		if (!std::isalpha(str[i]))
			return (false);
	return (true);
}
