/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_arg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:19:35 by aptive            #+#    #+#             */
/*   Updated: 2023/04/11 15:57:42 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/web_serv.hpp"

void check_arg(int argc, char ** argv)
{
	if (argc != 3)
		throw std::string("Error : Wrong number of arguments !\n");

	for (int i = 0; argv[1][i]; i++)
	{
		if (!isdigit(argv[1][i]))
			throw std::string("Error : Wrong format port !\n");
	}
	if (atoi(argv[1]) < 1 && atoi(argv[1]) > 65635)
		throw std::string("Error : Wrong port !\n");
}
