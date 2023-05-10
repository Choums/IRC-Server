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

void	Server::Display_Modes(User& user)
{
	std::cout << GREEN << "-Display User Modes-" << END << std::endl;
	std::string	server = "localhost";
	std::string	str = RPL_UMODEIS(server, user, user.getModes());
	std::cout << str << std::endl;
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

void	Server::Display_Chan_Modes(User& user, Channel const& channel)
{
	std::cout << GREEN << "-Display Channel Modes-" << END << std::endl;
	std::string	str = RPL_CHANNELMODEIS(user, channel.getName(), channel.getModes());
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

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
//	----
//	If no other parameter is given, then the server will return the current
//    settings for the nick.
void	Server::cmd_Mode(User& user, std::string const& rest)
{
	std::stringstream	ss(rest);
	std::string			target;	// Channel ou User
	std::string			mode;	// modif du mode
	ss >> target >> mode;
	std::string	str;

	std::cout << YELLOW << "-Mode Modification-" << END << std::endl;
	std::cout << RED << "<" << target << ">\n<" << mode << ">" << END << std::endl;
	
	// std::cout << RED << mode.size() << END << std::endl;
	if (target.size() == 1)
	{
		str = ERR_NEEDMOREPARAMS(user, "MODE");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return ;
	}

	if (mode.size() == 1 && target[0] != '#') // Display User modes
	{
		Display_Modes(user);
		return ;
	}

	if (target[0] == '#') // Channel modes
	{
		Chan_iter	it = this->get_Channel(target);
		if (it != this->_channel.end())
		{
			Channel *tmp = *it;
			
			if (tmp->Is_Present(user.getNickname()) || this->is_Ope(user))
			{
				std::string	nick; // Si l'user n'est pas donné, display channel modes
				ss >> nick;
				
				std::cout << RED << "<" << nick << ">" << END << std::endl;
				if (mode.size() == 1) {// Display Channel modes, /mode <channel>
						Display_Chan_Modes(user, *tmp); return ;}
				
				if (tmp->Is_Ope(user) || this->is_Ope(user)) // Chan oper ou Serv oper, Update Channel modes ou Channel User modes
				{
					if (nick.size() == 1) // Update Channel modes, /mode <channel> <mode>
					{
						std::cout << GREEN << "-Update Channel Modes-" << END << std::endl;
						tmp->setChanModes(mode);
						// Display_Chan_Modes(user, *tmp);
						return ;
					}
					
					User_iter itu = this->get_User(nick);
					
					if (itu != this->_client_socket_v.end()) // Update Channel User modes, /mode <channel> <mode> <nickname>
					{
						User *tmp_user = *itu;

						if (tmp->Is_Present(tmp_user->getNickname())) // Verifie que l'user est dans le channel
						{
							std::cout << GREEN << "-Update Channel User Modes-" << END << std::endl;
							tmp->setUserModes(user, *tmp_user, mode);
							if (!tmp->Is_OpePresent())
								this->setRmChannel(tmp);
						}
						else
						{
							str = ERR_NOUSERONCHANNEL(user, tmp->getName(), nick);
							send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
							return ;	
						}
					}
					else
					{
						str = ERR_NOSUCHNICK(user, nick);
						send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
						return ;	
					}
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
				str = ERR_NOTONCHANNEL(user, target);
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
		std::cout << "==>" << user.getNickname() << "<==" << std::endl;
		if (target == user.getNickname())
			user.setUserMode(mode);
		else
		{
			std::cout << RED << "=! Nick Mismatch !=" << END << std::endl;
			str = ERR_USERSDONTMATCH(user);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			return ;
		}
		str = USERMODE(user, user.getModes());
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	}
}