/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 18:43:35 by marvin            #+#    #+#             */
/*   Updated: 2023/05/04 18:43:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

//	MODE <channel> +o <nickname>
//	MODE <nickname> +o
//	*( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
// 	----
// ERR_NEEDMOREPARAMS
// ERR_USERSDONTMATCH
// ERR_UMODEUNKNOWNFLAG
// RPL_UMODEIS
//	----
//  Les serv oper ont des pouvoirs spécifiques sur le canal,
//	 tels que l'ajout ou le retrait des utilisateurs,
//	 la modification des modes du canal, etc.
//	----
// ERR_NOSUCHCHANNEL (403) : Le canal spécifié dans la commande "Mode" n'existe pas sur le serveur.
// ERR_CHANOPRIVSNEEDED (482) : L'utilisateur qui a envoyé la commande "Mode" n'a pas les privilèges suffisants 
//		pour modifier les modes du canal. Il doit être un opérateur sur le canal pour pouvoir le faire.
// ERR_UNKNOWNMODE (472) : La commande "Mode" a été envoyée avec un mode inconnu ou invalide pour le canal.
// ERR_NEEDMOREPARAMS (461) : La commande "Mode" a été envoyée avec des paramètres manquants.
//		Les paramètres nécessaires pour cette commande dépendent du mode que l'utilisateur souhaite modifier.
//	----
// A user MODE command MUST only be accepted if both the sender of the
//    message and the nickname given as a parameter are both the same.
void	Server::cmd_Mode(User& user, std::string const& rest)
{
	std::stringstream	ss(rest);
	std::string			target;	// Channel ou User
	std::string			mode;	// modif du mode
	ss >> target >> mode;
	std::string	str;

	std::cout << YELLOW << "-Mode Modification-" << END << std::endl;
	std::cout << RED << "<" << target << ">\n<" << mode << ">" << END << std::endl;
	
	if (target.empty() || mode.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "MODE");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	if (target[0] == '#') // Channel mode
	{
		Chan_iter	it = this->get_Channel(target);
		if (it != this->_channel.end())
		{
			Channel *tmp = *it;
			if (tmp->Is_Ope(user) || this->is_Ope(user))
			{
				std::string	nick;
				ss >> nick;
			}
			else
			{
				str = ERR_CHANOPRIVSNEEDED(user, target);
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
				return ;
			}
		}
		else
		{
			str = ERR_NOSUCHCHANNEL(user, target);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			return ;
		}
	}
	else // User mode
	{
		if (target == user.getNickname())
			user.setUserMode(mode);
		else
		{
			str = ERR_USERSDONTMATCH(user);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			return ;
		}
	}
}