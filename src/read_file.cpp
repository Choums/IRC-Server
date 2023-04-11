/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelauna <tdelauna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:19:55 by aptive            #+#    #+#             */
/*   Updated: 2023/04/10 14:22:30 by tdelauna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

// void parse_to_pair (std::string str)
std::pair<std::string, std::string> parse_to_pair (std::string str)
{
	std::string s1;
	std::string s2;

	int i = -1;
	while ( ++i < static_cast<int>(str.size()) && str[i] != ':')
		s1.append(str, i, 1);
	while ( str[++i] )
		s2.append(str, i, 1);
	std::pair<std::string, std::string> my_pair = std::make_pair(s1, s2);
	return my_pair;
}

std::map<std::string, std::string> open_and_check_conf_file(std::string file)
{
	std::ifstream	flux(file.c_str());
	std::map<std::string, std::string> list_conf;
	std::string line;

	if (!flux.is_open())
		throw std::string("Error : Unable to open the file !\n");
	while (!flux.eof() && getline(flux, line))
	{
		if (!line.empty())
		{
			parse_to_pair (line);
			list_conf.insert(parse_to_pair(line));
		}
	}
	return list_conf;
}
