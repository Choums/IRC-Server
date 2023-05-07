/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 08:47:29 by root              #+#    #+#             */
/*   Updated: 2023/05/07 13:49:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(User& user, std::string const& name)
{
	std::cout << "---------------------------------------" << std::endl;
	std::cout << "Creation d'un nouveau Canal: "<< GREEN << "<" << name << ">" << END << std::endl;
	std::cout << "---------------------------------------" << std::endl;

	this->setName(name);
	this->AddUser(user, true);
	this->_InvOnly = false;
}

Channel::~Channel()
{
	this->_users.clear();
	this->_privilege.clear();
	this->_ban.clear();
	this->_invited.clear();
}

void	Channel::Privmsg(User& user, std::string const& msg)
{
	std::map<int, User*>::iterator	it = this->_users.begin();
	std::map<int, User*>::iterator	ite = this->_users.end();

	std::string	privmsg = ":" + user.getUsername() + " PRIVMSG " + this->_name + msg + "\r\n";
	
	while (it != ite)
	{
		// str = PRIVMSGCHAN();
		send(it->first, privmsg.c_str(), privmsg.size(), MSG_NOSIGNAL);
		it++;
	}
}

// Send le msg a tout les users du Canal
void	Channel::Broadcast(std::string const& msg)
{
	std::map<int, User*>::iterator	it = this->_users.begin();
	std::map<int, User*>::iterator	ite = this->_users.end();

	while (it != ite)
	{
		send(it->first, msg.c_str(), msg.size(), MSG_NOSIGNAL);
		std::cout << RED<< "<" << this->_name << ">: BROADCAST TO " << it->first << ", " << it->second->getNickname()  << END << std::endl;
		it++;
	}
}

// Send le topic a tout les users du Canal, Command /topic
void	Channel::Broadcast_topic()
{
	std::map<int, User*>::iterator	it = this->_users.begin();
	std::map<int, User*>::iterator	ite = this->_users.end();

	std::string	topic;

	while (it != ite)
	{
		User	tmp = *(it->second);
		topic = RPL_TOPIC(tmp, this->_name, this->_topic);
		send(it->first, topic.c_str(), topic.size(), MSG_NOSIGNAL);
		it++;
	}
}

// Add un user au channel avec ses privileges (ope ou standard)
void	Channel::AddUser(User& new_user, bool priv) // check user existant
{
	if (!this->_topic.empty())
	{
		std::string	str = RPL_TOPIC(new_user, this->getName(), this->_topic);
		send(new_user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	}
	this->_users.insert(std::pair<int, User *>(new_user.getFd(), &new_user));
	this->_privilege.insert(std::pair<int, bool>(new_user.getFd(), priv));
	
	if (this->Is_Inv(new_user)) // New_user a accepté l'invitation, il est supp de la liste des invités
		this->_invited.erase(this->_invited.find(new_user.getFd()));
	std::string msg = ":" + new_user.getNickname() + "!"+ new_user.getNickname() +"@" + new_user.getHostname() + " JOIN " + this->_name + "\r\n";

	std::cout << GREEN << "< " << this->_name << " >: " << msg << END << std::endl;
	this->Broadcast(msg);
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
	this->_invited.insert(std::pair<int, bool>(new_user.getFd(), true));

	std::string str = RPL_INVITE(user, new_user, this->_name); // Invitation au new_user
	send(new_user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	
	str = RPL_INVITING(user, new_user, this->_name); // Message de confirmation a l'inviteur
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

// Ajoute un nouvel operateur de Canal
void	Channel::AddOpe(User& new_oper)
{
	this->setUserPrivilege(new_oper.getFd(), true);
	std::cout << RED << "<" << this->_name << ">: " << new_oper.getNickname() << " est devenu opetareur !" << END << std::endl;
}

// Retire les droits operateurs du Canal a l'user 
void	Channel::RmOpe(User& user)
{
	this->setUserPrivilege(user.getFd(), false);
	std::cout << RED << "<" << this->_name << ">: " << user.getNickname() << " n'est plus opetareur !" << END << std::endl;
}

//Commande part
// :<user> PART <channel> :<reason>
// :John!~user@host PART #channel :Je reviendrai plus tard
void	Channel::PartUser(User& user, std::string const& reason)
{
	this->_users.erase(this->_users.find(user.getFd())); // Supp de la list des users

	this->_privilege.erase(this->_privilege.find(user.getFd())); // Supp de la list des priv
		
	std::cout << YELLOW << "PART " << user.getUsername() << " has been deleted from " << this->_name << END << std::endl;
	this->getUsers();
	std::string	cast = ":" + user.getUsername() + "!user@localhost PART " + this->_name + " :" + reason + "\r\n";
	this->Broadcast(cast); // Display a tout les users que l'user est parti

	std::string	str = ":" + user.getUsername() + "!user@localhost PART " + this->_name + "\r\n";
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL); // Confirmation a l'user que la commande est reussi
}

bool	Channel::Is_Ban(User& user)
{
	std::map<int, bool>::iterator it = this->_ban.find(user.getFd());
	if (it == this->_ban.end())
		std::cout << GREEN << "-User is not banned-" << END << std::endl;
	else
		std::cout << RED << "-User is banned-" << END << std::endl;
	return (it->second);
}

bool	Channel::Is_Ope(User& user)
{
	std::map<int, bool>::iterator it = this->_privilege.find(user.getFd());
	if (it == this->_privilege.end())
		std::cout << RED << "-User is not operator-" << END << std::endl;
	else
		std::cout << GREEN << "-User is operator-" << END << std::endl;
	return (it->second);
}

bool	Channel::Is_Inv(User& user)
{
	std::map<int, bool>::iterator it = this->_invited.find(user.getFd());
	if (it == this->_invited.end())
		std::cout << RED << "-User is not invited-" << END << std::endl;
	else
		std::cout << GREEN << "-User is invited-" << END << std::endl;
	return (it->second);
}

bool	Channel::Is_Present(std::string const& user)
{
	if (this->getUser(user) == NULL)
		std::cout << RED << "-User is not  present-" << END << std::endl;
	else
		std::cout << GREEN << "-User is present-" << END << std::endl;
	return (this->getUser(user) ? true : false);
}

bool	Channel::Is_InvOnly() const
{
	return (this->_InvOnly ? true : false);
}

		/*	Getters */

std::string	Channel::getName() const
{	return (this->_name); }

std::string	Channel::getTopic() const
{	return (this->_topic); }

std::string	Channel::getModes() const
{
	std::string	modes;

	if (this->Is_InvOnly())
		modes.push_back('i');

	modes.push_back('b');

	std::cout << GREEN << "<" << this->_name << "> modes: [" << modes << "]" << END << std::endl;
	return (modes);
}

std::vector<User *>	Channel::getUsers()
{
	std::vector<User *>	list_user;

	for (std::map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		list_user.push_back(it->second);
		std::cout << it->second->getUsername() << std::endl;
	}
	return (list_user);	
}

size_t	Channel::getNumUsers() const
{
	return (this->_users.size());
}

std::string	Channel::getSNumUsers() const
{
	std::stringstream	ss;
	ss << this->_users.size();
	std::string	num = ss.str();
	return (num);
}

User*	Channel::getUser(std::string const& user)
{
	for (std::map<int, User*>::iterator	it = this->_users.begin(); it != this->_users.end(); it++)
	{
		User *tmp = it->second;	
		if (!user.compare(tmp->getNickname()))
			return(tmp);
	}
	return (NULL);
}

bool	Channel::getUserPrivilege(int user_fd) const
{	return ((this->_privilege.find(user_fd)->second)); }

		/*	Setters */

void	Channel::setName(std::string name)
{	this->_name = name; }

void	Channel::setChanModes(std::string const& mode)
{
	bool		sign;
	std::string	cast;
	
	size_t		i(0);
	while (i < mode.size())
	{
		if (mode[i] == '+')
			sign = true;
		else if (mode[i] == '-')
			sign = false;
		else if (mode[i] == 'i' && sign)
		{
			this->_InvOnly = true;
			this->getModes();
			cast = CHANMODE(this->_name, "+i");
			std::cout << RED << cast << END << std::endl;
			this->Broadcast(cast);
		}
		else if (mode[i] == 'i' && !sign)
		{
			this->_InvOnly = false;
			this->getModes();
			cast = CHANMODE(this->_name, "-i");
			this->Broadcast(cast);
		}
		else
		{
			// ERR_NOTIMPLEMENTED
		}
		i++;
	}
}

// /mode <channel> <mode> <nickname>
void	Channel::setUserModes(User& user, std::string const& mode)
{
	bool	sign;

	size_t	i(0);
	while (i < mode.size())
	{
		if (mode[i] == '+')
			sign = true;
		else if (mode[i] == '-')
			sign = false;
		else if (mode[i] == 'o' && sign)
		{
			if (!this->Is_Ope(user))
			{
				this->AddOpe(user);
				this->Broadcast(RPL_CHANNELMODEIS(user, this->_name, "+o"));
			}
		}
		else if (mode[i] == 'o' && !sign)
		{
			if (this->Is_Ope(user))
			{
				this->RmOpe(user);
				this->Broadcast(RPL_CHANNELMODEIS(user, this->_name, "-o"));
			}
		}
		else if (mode[i] == 'i' && sign)
		{
			
		}
		else if (mode[i] == 'i' && !sign)
		{
			
		}
		else if (mode[i] == 'b' && sign)
		{

		}
		else if (mode[i] == 'b' && !sign)
		{
			
		}
		i++;
	}
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
	this->Broadcast_topic();	
}

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