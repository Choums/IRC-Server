/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 08:47:29 by root              #+#    #+#             */
/*   Updated: 2023/04/28 19:29:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{
	std::cout << "---------------------------------------" << std::endl;
	std::cout << "Creation d'un nouveau Canal" << std::endl;
	std::cout << "---------------------------------------" << std::endl;
}

Channel::~Channel()
{
	this->_users.clear();
	this->_privilege.clear();
}

// Add un user au channel avec ses privileges (ope ou standard)
void	Channel::AddUser(User& new_user, bool priv) // check user existant
{
	if (this->_users.find(new_user.getFd()) != this->_users.end())
		return ;
		// throw UserAlreadyExists();
	
	this->_users.insert(std::pair<int, User *>(new_user.getFd(), &new_user));
	this->_privilege.insert(std::pair<int, bool>(new_user.getFd(), priv));
}

// Ajoute un nouvel User en tant qu'inviter dans le channel
// 341    RPL_INVITING "<channel> <nick>"
// Returned by the server to indicate that the
// attempted INVITE message was successful and is
// being passed onto the end client.
// -------
// Only the user inviting and the user being invited will receive
//	notification of the invitation.  Other channel members are not notified.
void	Channel::InvUser(User& user, User& new_user)
{	
	this->_users.insert(std::pair<int, User *>(new_user.getFd(), &new_user));
	this->_privilege.insert(std::pair<int, bool>(new_user.getFd(), false));
	this->_invited.insert(std::pair<int, bool>(new_user.getFd(), true));

	std::string	str = RPL_INVITING(user, this->_name, new_user);
	
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	send(new_user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

/*	Find l'user via son fd comme key et l'erase de la map
 *	@exception User not found.
*/
void	Channel::RmUser(int user_fd)
{
	std::map<int, User *>::iterator	it;
	
	it = this->_users.find(user_fd);
	
	if (it != this->_users.end())
		this->_users.erase(it);
	else
		return ;
		// throw UserNotFound();
	
	std::map<int, bool>::iterator	pit;
	pit = this->_privilege.find(user_fd);
	if (pit != this->_privilege.end())
		this->_privilege.erase(pit);
}

void	Channel::RmUser(std::string name)
{
	std::map<int, User *>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
		std::cout << it->second->getNickname() << std::endl;
		if (!name.compare(it->second->getNickname()))
			this->_users.erase(it);
		it++;
	}
	// if (it == this->_users.end())
	// 	return ;
		// throw UserNotFound();

	std::map<int, bool>::iterator	pit = this->_privilege.find(it->first);
	this->_privilege.erase(pit);

	std::cout << "[" << this->_name << "] : [" << it->second->getNickname() << "] has been removed" << std::endl;
}


bool	Channel::Is_Ban(User& user)
{
	std::map<int, bool>::iterator it = this->_ban.find(user.getFd());
	return (it->second);
}

bool	Channel::Is_Ope(User& user)
{
	std::map<int, bool>::iterator it = this->_privilege.find(user.getFd());
	return (it->second);
}

bool	Channel::Is_Inv(User& user)
{
	std::map<int, bool>::iterator it = this->_invited.find(user.getFd());
	return (it->second);
}

bool	Channel::Is_Present(std::string const& user)
{
	return (this->getUser(user) ? true : false);
}


		/*	Getters */

std::string	Channel::getName() const
{	return (this->_name); }

std::string	Channel::getTopic() const
{	return (this->_topic); }

std::vector<User *>	Channel::getUsers()
{
	std::vector<User *>	list_user;

	for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		list_user.push_back(it->second);
		std::cout << it->second->getNickname() << std::endl;
	}
	return (list_user);	
}

User*	Channel::getUser(std::string const& user)
{
	for (std::map<int, User*>::iterator	it = this->_users.begin(); it != this->_users.end(); it++)
		if (!user.compare(it->second->getNickname()))
			return(it->second);
	return (NULL);
}

bool	Channel::getUserPrivilege(int user_fd) const
{	return ((this->_privilege.find(user_fd)->second)); }


		/*	Setters */

void	Channel::setName(std::string name)
{	this->_name = name; }

void	Channel::setTopic(std::string topic)
{	this->_topic = topic; }

void	Channel::setUserPrivilege(int user_fd, bool priv)
{	(this->_privilege.find(user_fd))->second = priv; }

		/*	Displa Operator Overload */
		
/*	Affiche toutes les informations du Canal */
std::ostream&	operator<<(std::ostream& flux, Channel& cnl)
{
	std::vector<User *>	list_user = cnl.getUsers();
	flux << "---------------------------------------" << std::endl;
	flux << "Canal " << cnl.getName() << std::endl;
	flux << "Topic: " << cnl.getTopic() << std::endl;
	flux << "list_user:" << std::endl;
	for (std::vector<User *>::iterator it = list_user.begin(); it != list_user.end(); it++)
		flux << (*it)->getFd() << "\t" << (*it)->getNickname() << cnl.getUserPrivilege((*it)->getFd()) << std::endl;
	flux << "---------------------------------------" << std::endl;
	return (flux);
}


		/*	Comparison Operator Overload */

bool	operator==(Channel const& cnl1, Channel const& cnl2)
{
	if (!cnl1.getName().compare(cnl2.getName()))
		return (true);
	return (false);
}