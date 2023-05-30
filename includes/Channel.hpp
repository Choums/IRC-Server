/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 08:28:18 by root              #+#    #+#             */
/*   Updated: 2023/05/30 17:35:15 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class User;

class	Channel {
	public:
		Channel(User& user, std::string const& name, std::string const& pass);
		Channel(Channel const& cpy);
		Channel&	operator=(Channel const& obj);
		virtual ~Channel();

		void	Privmsg(User& user, std::string const& msg);
		void	Broadcast(std::string const& msg);
		void	Broadcast_topic();

		void	AddUser(User& new_user, bool priv);
		void	InvUser(User& user, User& new_user);
		void	UnInvUser(User& target);
		void	AddOpe(User& new_oper);
		void	RmOpe(User& user);
		void	PartUser(User& user, std::string const& reason);
		void	BanUser(User& user, User& target);
		void	UnBanUser(User& user, User& target);
		void	KickUser(User& user, User& target, std::string const& reason);
		bool	Is_PassValid(std::string const& pass);

		bool	Is_Ban(User& user);
		bool	Is_Ope(User& user);
		bool	Is_Inv(User& user);
		bool	Is_Present(std::string const& user);
		bool	Is_OpePresent();
		bool	Is_InvOnly() const;
		bool	Is_BanOnly() const;
		bool	Is_Private() const;
		bool	Is_limitSet() const;
		bool	Is_PassOnly() const;
		bool	Is_TopicLock() const;

		
				/*	Accesseurs */
		std::vector<User*>	getUsers();
		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getModes() const;
		void				getWho(User& user);
		std::string			getPass() const;
		int					getCapacity() const;
		int					getNumUsers() const;
		std::string			getSNumUsers() const;
		User*				getUser(std::string const& user);
		bool				getUserPrivilege(int user_fd) const;

		void				setName(std::string name);
		void				setChanModes(User& user, std::string const& mode, std::string const& arg);
		void				setUserModes(User& user, User& target, std::string const& mode);
		void				setTopic(std::string const& topic);
		void				setTopicClear();
		void				setUserPrivilege(int user_fd, bool priv);
		void				setChanLimit(int capacity);
		void				setChanPass(std::string const& pass);
		void				setTopLock(bool	lock);

	private:
		std::string				_name;		//	nom du canal
		std::string				_topic;		//	Definis le topic du Canal
		std::string				_pass;		//	Key permettant de rejoindre le Canal +k
		std::map<int, User *>	_users;		//	liste tout les users presents sur le canal ainsi que leur fd
		std::map<int, bool>		_privilege;	//	Definis les privileges des differents users: Operateur ou standard +o
		std::list<int>			_ban;		//	Liste des Users bannis du Canal +b
		std::list<int>			_invited;	//	Liste des Users invites du Canal +i
		int						_capacity;	//	Nombre d'Users pouvant faire partie du Canal +l
		bool					_InvOnly;	//	Mode du Canal, Invite Only
		bool					_bans;		//	Mode du Canal, Bans are active
		bool					_limit;		//	Mode du Canal, Users limit is set
		bool					_key;		//	Mode du Canal, Pass is necessary to join
		bool					_top;		//	Mode du Canal, Topic restriction
};

std::ostream&	operator<<(std::ostream& flux, Channel& cnl);
bool			operator==(Channel const& cnl1, Channel const& cnl2);
