/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:37:42 by aptive            #+#    #+#             */
/*   Updated: 2023/05/03 16:35:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/web_serv.hpp"
# include "../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Server::Server()
{
}

Server::Server(Server const & src)
{
	(void)src;
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
	fd_set read_sockets;
	FD_ZERO(&read_sockets);
	FD_SET(_server_fd, &read_sockets);

	_max_socket_fd = _server_fd;

	// Boucle principale du serveur
	int count(0);
	while (true)
	{
		std::cout << "[SERVER] : waiting data : " << YELLOW << count << END << std::endl;
		count++;
		if (count > 20)
			exit(EXIT_SUCCESS);

		// Wait for any of the sockets to become readable
		fd_set temp = read_sockets;
		if (select(_max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
			throw std::string("Error : waiting for sockets to become readable !\n");

		// traitement de l'activité sur les sockets
		this->gestion_new_connexion(&temp, &read_sockets, addr);

		affichage_vector(_client_socket_v);

			// Check for data on any of the client sockets
			this->gestion_activite_client( &read_sockets, &temp );

	}
}

bool	Server::verif_password(User& user, std::string const& pass)
{
	std::string	str;
	if (pass.empty())
	{
		str = ERR_NEEDMOREPARAMS(user, "PASS");
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return (false);
	}
	if (!this->_password.compare(pass))
	{
		std::cout << "|" << this->_password << "| == |" << pass << "|" << std::endl;
		str = ERR_PASSWDMISMATCH(user);
		send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		return (false);
	}
	str = "[SERVER] : Password correct, Connection established !\n";
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
	return (true);
}


void	Server::gestion_new_connexion(fd_set * temp, fd_set * read_sockets, struct sockaddr_in addr)
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

		FD_SET(new_socket, read_sockets);

		if (new_socket > _max_socket_fd)
			_max_socket_fd = new_socket;

		std::cout	<< "[SERVER] : New connexion : socket_fd : "
					<< new_socket << "/ ip : " << inet_ntoa(addr.sin_addr)
					<< " / port : " << ntohs(addr.sin_port)
					<< std::endl;

		_client_socket_v.push_back(User(new_socket));

		sendMessage(new_socket, "[SERVER] : password ?\n");


	}
}



void Server::gestion_activite_client(fd_set * read_sockets, fd_set * temp)
{
	int valread;
	char buffer[1025];
	(void)read_sockets;

	for (size_t i = 0; i < _client_socket_v.size(); i++)
	{
		int client_socket_fd = _client_socket_v[i].getFd();
		if (FD_ISSET(client_socket_fd, temp))
		{
			valread = read(client_socket_fd, buffer, 1024);
			std::string buf (buffer, valread);

			if (valread == 0)
			{

				std::cout << RED << "[SERVER] : Delete " << _client_socket_v[i].getNickname() << END << std::endl;
				// Client disconnected, remove from active socket set
				sendMessageWarning(_client_socket_v[i].getFd(), "[SERVER] : You have been disconnected\n");
				close(client_socket_fd);
				FD_CLR(client_socket_fd, read_sockets);
				_client_socket_v.erase(_client_socket_v.begin()+i);

			}
			else
			{
				// std::cout << "HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
				_client_socket_v[i].setBuf(buf);
				this->parsing_cmd( &_client_socket_v[i]);
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
		std::cout << RED << cmd << std::endl << rest << END << std::endl;
		std::cout << "-------------------------------------->\n";

		this->handleCommandServer(cmd, rest, *user);

	}

	user->clearBuf();
}

static Command option(const std::string& cmd)
{
	if (!cmd.compare("CAP")) return (Cap);
	if (!cmd.compare("PASS")) return (Pass);
	if (!cmd.compare("NICK")) return (Nick);
	if (!cmd.compare("JOIN")) return (Join);
	if (!cmd.compare("NAMES")) return (Names);
	if (!cmd.compare("LIST")) return (List);
	if (!cmd.compare("WHOIS")) return (Whois);
	if (!cmd.compare("PART")) return (Part);
	if (!cmd.compare("PING")) return (Ping);
	if (!cmd.compare("USER")) return (User_cmd);
	if (!cmd.compare("INVITE")) return (Invite);
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
		case Names:
			this->commandeServer_name(user);
			break;
		case List:
			this->cmd_List(user, rest);
			break;
		case Whois:
			this->cmd_Whois(user, rest);
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
	}
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

void	Server::commandeServer_name( const User & user )
{
	// std::cout << "commandeServer_name\n";
	for (size_t i = 0; i < _client_socket_v.size(); i++)
	{

		const std::string message = _client_socket_v[i].getNickname() + "\n";
		user.sendMessage(message);
	}
}

/*
 *	Command LIST
 *	Permet d'afficher tout les channels existant sur le serveur
 *	Concatene tout les noms de channel avec un nl en tant que separateur
 *	La list des channels est send a l'user
 *
 * The list command is used to list channels and their topics.  If the
 * <channel> parameter is used, only the status of that channel is
 * displayed.
 * 322 MonPseudo #channel 10 :Discussion générale
 * RPL_LIST
 * RPL_LISTEND
 * Si le Canal demandé n'existe pas, rien n'est renvoyé. Seul les Canaux existant seront renvoyer via RPL_LIST
*/
void	Server::cmd_List(User& user, std::string const& rest)
{
	std::cout << YELLOW << "--- LIST ---" << END << std::endl;
	std::string	str;
	// std::cout << RED << "|" << rest.size() << "|" << END << std::endl;
	if (rest.size() == 2) // Display tout les Canaux du serveur
	{
		std::cout << GREEN << "[Display All Channels]" << END << std::endl;
		for (size_t i(0); i < this->_channel.size(); i++)
		{
			str = RPL_LIST(user, this->_channel[i], this->_channel[i]->getTopic());	
			std::cerr << RED << str << END << std::endl;
			send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
		}	
	}
	else // Display uniquement les Canaux demandés et existant
	{
		std::cout << GREEN << "[Display Asked Channels]" << END << std::endl;
		std::vector<std::string>	list_channels =	parse_cnl_name(rest);
		
		for (size_t i(0); i < list_channels.size(); i++)
		{
			Chan_iter	tmp = this->get_Channel(list_channels[i]);
			if (tmp != this->_channel.end())
			{
				str = RPL_LIST(user, (*tmp), (*tmp)->getTopic());	
				send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
			}
		}
	}
	str = RPL_LISTEND(user);	
	send(user.getFd(), str.c_str(), str.size(), MSG_NOSIGNAL);
}

/*
 *	Command WHOIS <target>
 *	Permet d'afficher la liste des channels dont fait partie le <target>
 *	cherche le <target> dans la liste des users presents dans le serveur
 *	Concatene tout ses channel avec un nl en tant que separateur
 *	La list des channels est send a l'user
*/
void	Server::cmd_Whois(User const& user, std::string const& target) const
{
	std::cout << "target: " << target << std::endl;
	(void)user;
	for (size_t i(0); i < this->_client_socket_v.size(); i++)
	{
		if (!target.compare(this->_client_socket_v[i].getNickname()))
		{			
			// std::string	list = this->_client_socket_v[i].getListCnl();
			// user.sendMessage(list);
			return ;
		}
	}
	// throw std::string("Unknown User");
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
		if (!user.compare(it->getNickname()))
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
	Chan_iter	it = this->get_Channel(cnl->getName());

	delete cnl;	
	this->_channel.erase(it);	
}

void	Server::setNewChannel(Channel* cnl)
{
	this->_channel.push_back(cnl);
}


/* ************************************************************************** */
