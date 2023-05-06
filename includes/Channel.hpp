/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 08:28:18 by root              #+#    #+#             */
/*   Updated: 2023/05/06 13:01:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "web_serv.hpp"

class User;

class	Channel {
	public:
		Channel(User& user, std::string const& name);
		virtual ~Channel();

		void	Privmsg(User& user, std::string const& msg);
		void	Broadcast(std::string const& msg);
		void	Broadcast_topic();

		void	AddUser(User& new_user, bool priv);
		void	InvUser(User& user, User& new_user);
		void	AddOpe(User& new_oper);
		void	RmOpe(User& user);
		void	PartUser(User& user, std::string const& reason);
		void	BanUser();
		void	UnBanUser();
		void	KickUser();

		// void	Change_mode();

		// void	RmUser(int user_fd);
		// void	RmUser(std::string name);
		bool	Is_Ban(User& user);
		bool	Is_Ope(User& user);
		bool	Is_Inv(User& user);
		bool	Is_Present(std::string const& user);
		bool	Is_InvOnly() const;

				/*	Accesseurs */
		std::vector<User*>	getUsers();
		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getModes() const;
		size_t				getNumUsers() const;
		std::string			getSNumUsers() const;
		User*				getUser(std::string const& user);
		bool				getUserPrivilege(int user_fd) const;

		void				setName(std::string name);
		void				setChanModes(std::string const& mode);
		void				setUserModes(User& user, std::string const& mode);
		void				setTopic(std::string topic);
		void				setUserPrivilege(int user_fd, bool priv);
		
		
	private:
		std::string				_name;		//	nom du canal
		std::string				_topic;		//	Definis le topic du Canal
		std::map<int, User *>	_users;		//	liste tout les users presents sur le canal ainsi que leur fd
		std::map<int, bool>		_privilege;	//	Definis les privileges des differents users: Operateur ou standard +o
		std::map<int, bool>		_ban;		//	Liste des Users bannis du Canal +b
		std::map<int, bool>		_invited;	//	Liste des Users invites du Canal +i
		bool					_InvOnly;	//	Mode du Canal, Invite Only
		// Channel(Channel const& cpy);
		// Channel&	operator=(Channel const& obj);

	// public: /*	Exception */
	// 	class UserNotFound : std::exception {
	// 		public:
	// 			UserNotFound() throw() {}
	// 			virtual ~UserNotFound() throw() {}
	// 			virtual const char* what() const throw()
	// 			{	return ("User not found !"); }
	// 	};
	// 	class UserAlreadyExists : std::exception {
	// 		public:
	// 			UserAlreadyExists() throw() {}
	// 			virtual ~UserAlreadyExists() throw() {}
	// 			virtual const char* what() const throw()
	// 			{	return ("User already exists !"); }
	// 	};
};

std::ostream&	operator<<(std::ostream& flux, Channel& cnl);
bool			operator==(Channel const& cnl1, Channel const& cnl2);