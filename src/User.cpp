/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:50:55 by aptive            #+#    #+#             */
/*   Updated: 2023/04/14 17:20:38 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/User.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
User::User()
{

}

User::User(User const & src) : _fd(src.getFd()), _id(src.getId()), _nickname(src.getNickname())
{
}

User::User(int fd) : _fd(fd)
{
	// init _id
	static int id = 0;
	id++;
	this->_id = id;

	// init _nickname
	std::stringstream ss;
	ss << "Default_Nickname_" << id;
	this->_nickname = ss.str();

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

User::~User()
{

}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

User & User::operator=(User const & rhs)
{
	std::cout << "here\n";
	if (this != &rhs && rhs.getFd())
		*this = rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, User const & i )
{
	o << "User		:	" << i.getId() << std::endl;
	o << "fd		:	" << i.getFd() << std::endl;
	o << "Nickname	:	" << i.getNickname() << std::endl;
	return o;
}



/*
** --------------------------------- METHODS ----------------------------------
*/

void	User::handleCommand(const std::string& cmd, const std::string& rest)
{
	std::string levels[4] = { "/nick", "/join" };

	void (User::*f[2])(const std::string&) = {
		&User::setNickname
	};

	for (int i = 0; i < 2; i++) {
		if (levels[i] == cmd) {
			(this->*f[i])(rest);
		}
	}
}

void	User::sendMessage(const std::string& message)
{
	send(this->getFd(), message.c_str(), message.length(), 0);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int User::getFd( void ) const
{
	return this->_fd;
}

int User::getId( void ) const
{
	return this->_id;
}

std::string		User::getNickname(void) const
{
	return this->_nickname;
}

/*
** --------------------------------- SETTER ---------------------------------
*/

void User::setNickname(const std::string& rest)
{
	std::cout << "setNickname : " << std::endl;
	// std::cout << "setNickname : " << nickname << std::endl;
	this->_nickname = rest;


	std::string message = "Server : Your nickname has been changed to " + rest;
	this->sendMessage(message);
}



/* ************************************************************************** */
