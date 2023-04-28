/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 08:28:18 by root              #+#    #+#             */
/*   Updated: 2023/04/28 19:14:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "web_serv.hpp"

class User;

class	Channel {
	public:
		Channel();
		virtual ~Channel();


		void	AddUser(User& new_user, bool priv);
		void	InvUser(User& user, User& new_user);
		void	AddOpe(User& user);
		void	RmOpe(User& user);
		void	BanUser();
		void	UnBanUser();
		void	KickUser();
		void	RmUser(int user_fd);
		void	RmUser(std::string name);
		// void	sendMessage();
		bool	Is_Ban(User& user);
		bool	Is_Ope(User& user);
		bool	Is_Inv(User& user);
		bool	Is_Present(std::string const& user);

				/*	Accesseurs */
		std::string			getName() const;
		std::string			getTopic() const;
		std::vector<User*>	getUsers();
		User*				getUser(std::string const& user);
		bool				getUserPrivilege(int user_fd) const;

		void				setName(std::string name);
		void				setTopic(std::string topic);
		void				setUserPrivilege(int user_fd, bool priv);
		
		
	private:
		std::string				_name;		//	nom du canal
		std::string				_topic;		//	Definis le topic du Canal
		std::map<int, User *>	_users;		//	liste tout les users presents sur le canal ainsi que leur fd
		std::map<int, bool>		_privilege;	//	Definis les privileges des differents users: Operateur ou standard
		std::map<int, bool>		_ban;		//	Liste des Users bannis du Canal
		std::map<int, bool>		_invited;	//	Liste des Users invites du Canal
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