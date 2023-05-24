/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:37:42 by aptive            #+#    #+#             */
/*   Updated: 2023/05/24 17:09:45 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/ft_irc.hpp"
# include "../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Server::Server()
{
}

Server::Server(Server const & src)
{
	*this = src;
}

Server&	Server::operator=(Server const& obj)
{
	if (this != &obj)
	{
		_hostname	= obj._hostname;
		_server_fd	= obj._server_fd;
		_addr		= obj._addr;
		_port		= obj._port;
		_password	= obj._password;
		_max_socket_fd	= obj._max_socket_fd;
		_client_socket_v	= obj._client_socket_v;
		_channel	= obj._channel;
		_read_sockets	= obj._read_sockets;
	}
	return (*this);
}

Server::Server(int port, std::string password)
{
	std::cout << "[SERVER] Configuration server en cours ..." << std::endl;

	this->_hostname = "localhost";
	this->_port = port;
	this->_password = password;

	this->creation_socket_server(&this->_server_fd);
	this->configuration_socket_server(&this->_server_fd);
	this->creation_address_connexion(port, &this->_addr);
	this->association_socket_to_address(&this->_server_fd, &this->_addr);
	this->mode_listing_socket(&this->_server_fd);

	std::cout << "[SERVER] Configuration server succesful" << std::endl;

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	Chan_iter	it = this->_channel.begin();
	Chan_iter	ite = this->_channel.end();

	while (it != ite)
	{
		delete *it;
		it++;
	}

	User_iter	itu = this->_client_socket_v.begin();
	User_iter	iteu = this->_client_socket_v.end();

	while (itu != iteu)
	{
		delete *itu;
		itu++;
	}
	close(this->_server_fd);
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Server & Server::operator=(Server const & rhs)
// {
// 	if (this != &rhs && rhs.getFd())
// 		*this = rhs;
// 	return *this;
// }

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << GREEN <<"---------------- INFO SERVER ------------------------------" << END << std::endl;
	o << "_server_fd			:	" << i.getServer_fd() << std::endl;
	o << "_port				:	" << i.getPort() << std::endl;
	o << "_password			:	" << i.getPassword() << std::endl;
	// o << "_addr			:	" << &i.getAddr() << std::endl;
	o << GREEN <<"-------------- FIN INFO SERVER ----------------------------" << END << std::endl;

	return o;
}



/*
** --------------------------------- METHODS ----------------------------------
*/
void	Server::creation_socket_server( int * server_fd )
{
	// Creation socket file descriptor
	*server_fd = socket (AF_INET, SOCK_STREAM, 0);
	if (*server_fd < 0)
		throw std::string("Error : Error during the creation of socket !\n");
}

void	Server::configuration_socket_server( int *server_fd )
{
	int opt = 1;
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw std::string("Error : Error during the configuration of socket !\n");
}

void	Server::creation_address_connexion( int port , struct sockaddr_in * addr)
{
	// Création de l'adresse de connexion

	std::memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;
}

void	Server::association_socket_to_address (int *server_fd, struct sockaddr_in * addr)
{
	// Association de l'adresse de connexion au socket
	if (bind(*server_fd, (struct sockaddr*)addr, sizeof(*addr)) < 0)
		throw std::string("Error : Error during the association of socket !\n");
}

void	Server::mode_listing_socket(int *server_fd)
{
	// Mise en écoute du socket
	if (listen(*server_fd, SOMAXCONN) == -1)
		throw std::string("Error : Error during the listing of socket !\n");
}

void	Server::boucle_server( void )
{
	std::vector<User> client_socket_v;
	struct sockaddr_in addr = this->getAddr();

	// Initialize the read sockets set
	// fd_set _read_sockets;
	FD_ZERO(&_read_sockets);
	FD_SET(_server_fd, &_read_sockets);

	_max_socket_fd = _server_fd;

	// Boucle principale du serveur
	int count(0);
	g_signal = true;
	while (g_signal == true)
	{
		std::cout << "[SERVER] : waiting data : " << YELLOW << count << END << std::endl;
		count++;

		// Wait for any of the sockets to become readable
		fd_set temp = _read_sockets;
		if (select(_max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
			throw std::string("Error : waiting for sockets to become readable !\n");

		// traitement de l'activité sur les sockets
		this->gestion_new_connexion(&temp, &_read_sockets, addr);

		affichage_vector(_client_socket_v);

			// Check for data on any of the client sockets
			this->gestion_activite_client( &_read_sockets, &temp );

	}
}

bool    Server::verif_password(User& user, std::string const& mdp)
{
    std::string	str;
	std::string	pass = mdp;

	RmNewLine(pass, '\n');
	RmNewLine(pass, '\r');

    if (mdp.size() == 1)
    {
        str = ERR_NEEDMOREPARAMS(user, "PASS");
        send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
        return (false);
    }
    if (this->_password.compare(pass))
    {
        std::cout << this->_password.size() << "|" << this->_password << "| == "<< pass.size() <<"|" << pass << "|" << std::endl;
        str = ERR_PASSWDMISMATCH(user);
        send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
        return (false);
    }
    str = "[SERVER] : Password correct, Connection established !\n";
    send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
    user.setAuth_passwordOK();
    return (true);
}


void	Server::gestion_new_connexion(fd_set * temp, fd_set * _read_sockets, struct sockaddr_in addr)
{
	if (FD_ISSET(_server_fd, temp))
	{
		struct sockaddr_in client_address;
		int addrlen = sizeof(client_address);

		int new_socket = accept(_server_fd,
								(struct sockaddr *)&client_address,
								(socklen_t *)&addrlen
								);

		if (new_socket < 0)
			throw std::string("Error : Failed to accept connection !\n");

		FD_SET(new_socket, _read_sockets);

		if (new_socket > _max_socket_fd)
			_max_socket_fd = new_socket;

		std::cout	<< "[SERVER] : New connexion : socket_fd : "
					<< new_socket << "/ ip : " << inet_ntoa(addr.sin_addr)
					<< " / port : " << ntohs(addr.sin_port)
					<< std::endl;
		_client_socket_v.push_back(new User(new_socket));
	}
}



void Server::gestion_activite_client(fd_set * _read_sockets, fd_set * temp)
{
	int valread;
	char buffer[1025];
	(void)_read_sockets;

	for (size_t i = 0; i < _client_socket_v.size(); i++)
	{
		int client_socket_fd = _client_socket_v[i]->getFd();
		if (FD_ISSET(client_socket_fd, temp))
		{
			valread = read(client_socket_fd, buffer, 1024);
			std::string buf (buffer, valread);

			if (valread == 0)
			{
				
				std::cout << RED << "[SERVER] : Delete " << _client_socket_v[i]->getNickname() << END << std::endl;
				this->setRmUser(*this->_client_socket_v[i]);

				delete this->_client_socket_v[i];
				// Client disconnected, remove from active socket set

			}
			else
			{
				int buf_len = buf.size();
				if (buf[buf_len - 1] == '\n')
				{
					std::cout << "Commande complete\n";
					_client_socket_v[i]->setBuf(buf);
					this->parsing_cmd(_client_socket_v[i]);
				}
				else
				{
					std::cout << "add to buf\n";
					_client_socket_v[i]->setBuf(buf);
				}
			}
		}
	}
}

std::pair<std::string, std::string>  first_word(std::string line)
{
	std::istringstream iss(line);
	std::string word;
	std::string	rest;

	while (std::getline(iss, word, '\n'))
	{
		std::istringstream iss_word(word);
		iss_word >> word;
		if (!std::getline(iss_word, rest) || word.size() == 1 || word.empty())
			return (std::make_pair(word, ""));
		rest = iss.str().substr(word.size() + 1);
	}
	return (std::make_pair(word, rest));
}

// void Server::parsing_cmd( User * user )
// {
// 	std::string cmd;
// 	std::string rest;
// 	std::vector<std::string> v_parse;


// 	// for (size_t i = 0; i < user->getBuf().size(); i++)
// 	// 	std::cout << (int)user->getBuf()[i] << std::endl;
// 	// std::cout << "END buffer" << std::endl;

// 	v_parse = split_string(user->getBuf());

// 	// Affiche chaque élément du vecteur tokens
// 	// for (std::vector<std::string>::iterator it = v_parse.begin(); it != v_parse.end(); ++it) {
// 	// 	std::cout << *it << "|" << std::endl;
// 	// }

// 	if (v_parse[0][0] == '/')
// 	{
// 		std::cout << "[SERVER] : It's a commande: [" << v_parse[0] << "] !" << std::endl;
// 		this->handleCommandServer(v_parse[0], v_parse[1], *user);
// 		// user->handleCommand(v_parse[0], v_parse[1]);

// 	}

// 	user->clearBuf();
// }


void Server::parsing_cmd( User * user )
{
	std::string line;
	std::istringstream stream(user->getBuf());  // créer un flux d'entrée à partir de la chaîne de caractères

	while (std::getline(stream, line))
	{
		std::pair<std::string, std::string>	arg;
		std::string cmd;
		std::string	rest;
		std::cout << "-------------------------------------->\n";
		arg = first_word(line);
		cmd = arg.first;
		rest = arg.second;
		// if (cmd.empty())
		std::cout << RED << cmd << std::endl << rest << END << std::endl;
		std::cout << "-------------------------------------->\n";

		this->handleCommandServer(cmd, rest, *user);

	}

	user->clearBuf();
}

void	Server::send_privmsg(User& user, User& target, std::string const& msg)
{
	std::cout << YELLOW << "-Sending Message To "<< target.getNickname() << "-" << END << std::endl;

	std::cout << GREEN << "|" << msg << "|" << END << std::endl;
	std::string	privmsg = ":" + user.getNames() + " PRIVMSG " + target.getNickname() + " :" + msg + "\r\n";

	send(target.getFd(), privmsg.c_str(), privmsg.size(), MSG_NOSIGNAL);
}

static Command option(const std::string& cmd)
{
	if (!cmd.compare("CAP")) return (Cap);
	if (!cmd.compare("PASS")) return (Pass);
	if (!cmd.compare("NICK")) return (Nick);
	if (!cmd.compare("JOIN")) return (Join);
	if (!cmd.compare("WHO")) return (Who);
	if (!cmd.compare("LIST")) return (List);
	if (!cmd.compare("PART")) return (Part);
	if (!cmd.compare("PING")) return (Ping);
	if (!cmd.compare("USER")) return (User_cmd);
	if (!cmd.compare("INVITE")) return (Invite);
	if (!cmd.compare("MODE")) return (Mode);
	if (!cmd.compare("KICK")) return (Kick);
	if (!cmd.compare("PRIVMSG")) return (Privmsg);
	if (!cmd.compare("TOPIC")) return (Topic);
	if (!cmd.compare("NOTICE")) return (Notice);
	return (Unknown);
}



void	Server::handleCommandServer(std::string const& cmd, std::string const& rest, User& user)
{
	switch (option(cmd))
	{
		case Unknown:
			// throw std::string("Unknown Command !");
			break;
		case Cap:
			break;
		case Pass:
			this->verif_password(user, rest);
			break;
		case Nick:
			this->cmd_Nick(user, rest);
			break;
		case Join:
			this->cmd_JoinChannel(rest, user);
			break;
		case Who:
			this->cmd_Who(user, rest);
			break;
		case List:
			this->cmd_List(user, rest);
			break;
		case Part:
			this->cmd_Part(user, rest);
			break;
		case Ping:
			this->cmd_Ping(user, rest);
			break;
		case User_cmd:
			this->cmd_User(user, rest);
			break;
		case Invite:
			this->cmd_Invite(user, rest);
			break;
		case Mode:
			this->cmd_Mode(user, rest);
			break;
		case Kick:
			this->cmd_Kick(user, rest);
			break;
		case Privmsg:
			this->cmd_Privmsg(user, rest);
			break;
		case Topic:
			this->cmd_Topic(user, rest);
			break;
		case Notice:
			this->cmd_Notice(user, rest);
			break;
	}
}

bool	Server::is_Ope(User& user)
{
	return (user.getOper() ? true : false);
}

bool	Server::channel_exist(std::string const& cnl_name)
{
	for (size_t i(0); i < this->_channel.size(); i++)
		if (!cnl_name.compare(this->_channel[i]->getName()))
			return (true);
	return (false);
}


/*
** --------------------------------- COMMANDE ---------------------------------
*/
// 352    RPL_WHOREPLY
//               "<channel> <user> <host> <server> <nick>
//               ( "H" / "G" > ["*"] [ ( "@" / "+" ) ]
//               :<hopcount> <real name>"
void	Server::cmd_Who(User& user, std::string const& rest)
{
	std::string			str;
	std::stringstream	ss(rest);
	std::string			target;
	ss >> target;

	if (target[0] == '#') // Display tout les users du Canal
	{
		Chan_iter	it = this->get_Channel(target);
		if (it != this->_channel.end())
		{
			Channel	*channel = *it;
			if (!channel->Is_Ban(user))
			{
				// channel->getWho(user);
			}
		}
		else
		{
			str = ERR_NOSUCHCHANNEL(user, target);
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			return ;
		}
	}
	else // Display tout les users du serv
	{

	}

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Server::getHostname() const
{
	return (this->_hostname);
}

int	Server::getServer_fd() const
{
	return this->_server_fd;
}

struct sockaddr_in	Server::getAddr() const
{
	return this->_addr;
}

int	Server::getPort() const
{
	return this->_port;
}

std::string	Server::getPassword() const
{
	return this->_password;
}

User_iter	Server::get_User(std::string const& user)
{
	User_iter it = this->_client_socket_v.begin();
	User_iter ite = this->_client_socket_v.end();

	while (it != ite)
	{
		if (!user.compare((*it)->getNickname()))
			return (it);
		it++;
	}
	return (ite);
}

Chan_iter Server::get_Channel(std::string const& channel)
{
	Chan_iter it = this->_channel.begin();
	Chan_iter ite = this->_channel.end();

	std::cout << RED << "-Looking for channel-" << END << std::endl;
	while (it != ite)
	{
		Channel *tmp = *it;
		if (!channel.compare(tmp->getName()))
		{
			// std::cout <<GREEN << "found => " << tmp->getName() << END << std::endl;
			return (it);
		}
		it++;
	}
	return (ite);
}

/*
** --------------------------------- SETTER ---------------------------------
*/
void	Server::setServer_fd(const int & server_fd)
{
	this->_server_fd = server_fd;
}

void 	Server::setAddr(const struct sockaddr_in & addr)
{
	this->_addr = addr;
}

void	Server::setPort(const int & port)
{
	this->_port = port;
}

void	Server::setPassword(const std::string & password)
{
	this->_password = password;
}

void	Server::setRmChannel(Channel* cnl)
{
	Chan_iter	it = this->_channel.begin();

	std::cout << RED << "-Fermeture channel " << cnl->getName() << "-" << END << std::endl;

	while (it != this->_channel.end())
	{
		if (*it == cnl)
		{
			this->_channel.erase(it);
			delete cnl;
			std::cout << GREEN << "-Fermeture successful-" << END << std::endl;
			return ;
		}
		it++;
	}
}

void	Server::setRmUser(User &user)
{
	size_t i(0);
	while (i < this->_client_socket_v.size())
	{
		if (user.getFd() == this->_client_socket_v[i]->getFd())
			break;
		i++;
	}
	int fd = this->_client_socket_v[i]->getFd();

	std::cout << RED << "[SERVER] : Delete " << _client_socket_v[i]->getNickname() << END << std::endl;
	// Client disconnected, remove from active socket set
	close(fd);
	FD_CLR(fd, &_read_sockets);
	_client_socket_v.erase(_client_socket_v.begin()+i);
	
}

void	Server::setNewChannel(Channel* cnl)
{
	this->_channel.push_back(cnl);
}


/* ************************************************************************** */
