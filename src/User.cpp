/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:50:55 by aptive            #+#    #+#             */
/*   Updated: 2023/04/21 13:02:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/User.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
User::User()
{

}

User::User(User const & src) : _fd(src.getFd()), _id(src.getId()), _nickname(src.getNickname()), _auth_password(src.getAuth_password())
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

	// init _admin

	this->_admin = false;
	this->_auth_password = false;
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
	if (this != &rhs && rhs.getFd())
		*this = rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, User const & i )
{
	o << "User		:	" << i.getId() << std::endl;
	o << "fd		:	" << i.getFd() << std::endl;
	o << "Nickname	:	" << i.getNickname() << std::endl;
	if (i.getAdmin() == true)
		o << "Operator	:	" << "admin" << std::endl;
	else
		o << "Operator	:	" << "user" << std::endl;
	return o;
}



/*
** --------------------------------- METHODS ----------------------------------
*/



// void	User::handleCommand(const std::string& cmd, const std::string& rest)
// {
// 	// std::string levels[] = {
// 	// 							"/NICK",
// 	// 							"/JOIN",
// 	// 							"/NAMES" };

// 	void (User::*f[])(const std::string&) = {
// 		&User::setNickname,
// 		&User::cmd_JoinChannel
// 	};

// 	// for (int i = 0; i < 2; i++) {
// 	// 	if (levels[i] == cmd) {
// 	// 		(this->*f[i])(rest);
// 	// 	}
// 	// }

// 	switch (option(cmd))
// 	{
// 		case Unknown:
// 			throw std::string("Unknown Command !");
// 			break;
// 		case Nick:
// 			(this->*f[Nick - 1])(rest);
// 			break;
// 		case Join:
// 			(this->*f[Join - 1])(rest);
// 			break;
// 	}
// }

void	User::sendMessage(const std::string& message) const
{
	std::cout << "[SEND] : to [" << _nickname << "] : " << message;
	send(this->getFd(), message.c_str(), message.length(), 0);
}

void	User::setBuf(const std::string& buf)
{
	this->_buf += buf;

	if (this->_buf[this->_buf.size() - 1] == '\n')
		this->_buf[this->_buf.size() - 1] = '\0';

}

void	User::clearBuf( void )
{
	this->_buf = "";
}

/*
 *	Quitte tout les Canals dont l'user fait parti
 *	Parcours la liste des Canals, pour chaque Canal -> RmCnlMembership
*/
void	User::LeaveCnls()
{
	std::cout << "list part of cnl " << this->getListCnl() << std::endl;
	for (std::vector<Channel>::iterator it = this->_list_cnl.begin(); it != this->_list_cnl.end(); it++)
	{
		it->RmUser(this->_nickname);
		this->_list_cnl.erase(it);
	}
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

std::string		User::getBuf(void) const
{
	return this->_buf;
}

bool		User::getAdmin(void) const
{
	return this->_admin;
}

bool		User::getAuth_password(void) const
{
	// std::cout << "here : " << this->_auth_password << std::endl;
	return this->_auth_password;
}

// Renvoie une string avec tout les channels dont est present l'user
std::string	User::getListCnl() const
{
	std::string list_channel;
	std::cout << "list: [ ";
	for (size_t i(0); i < this->_list_cnl.size(); i++)
	{
		std::cout << this->_list_cnl[i].getName() << " ";
		list_channel.append(this->_list_cnl[i].getName());
		list_channel.push_back('\n');
	}
	std::cout << "]" << std::endl;
	return (list_channel);
}

/*
** --------------------------------- SETTER ---------------------------------
*/

void User::setNickname(const std::string& rest)
{
	// std::cout << "setNickname : " << std::endl;
	// std::cout << "setNickname : " << nickname << std::endl;
	this->_nickname = rest;


	std::string message = "Server : Your nickname has been changed to " + rest + "\n";
	this->sendMessage(message);
}

void	User::setAdmin(const bool & admin)
{
	this->_admin = admin;
}


void	User::setAuth_passwordOK( void )
{
	this->_auth_password = true;
}

// Ajoute le nouveau channel a la liste des channels dont l'user fait partie
void	User::setAddListCnlMember(Channel& cnl)
{
	this->_list_cnl.push_back(cnl);
}

// Retire le channel de la liste des channels dont l'user fait partie
// L'user est par la meme occassion retirer au sein du channel
void	User::setRmCnlMembership(Channel& cnl)
{
	cnl.RmUser(this->getNickname());
	for (std::vector<Channel>::iterator it = this->_list_cnl.begin(); it != this->_list_cnl.end(); it++)
	{
		if (*it == cnl)
		{
			this->_list_cnl.erase(it);
			return ;
		}
	}
	// throw std::string("Channel not found");
}

// // Retire le channel de la liste des channels dont l'user fait partie
// // L'user est par la meme occassion retirer au sein du channel
// void	User::setRmCnlMembership(Channel& cnl)
// {
// 	cnl.RmUser(this->getFd());
// }

/* ************************************************************************** */
