/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:37:42 by aptive            #+#    #+#             */
/*   Updated: 2023/04/20 19:48:36 by root             ###   ########.fr       */
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

void	verif_password(void)
{

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


	for (size_t i = 0; i < _client_socket_v.size(); i++)
	{
		int client_socket_fd = _client_socket_v[i].getFd();
		if (FD_ISSET(client_socket_fd, temp))
		{
			valread = read(client_socket_fd, buffer, 1024);
			std::string buf (buffer, valread);
			// std::cout << "valread on client socket : " << valread << " / "<< client_socket_fd << std::endl;

			std::cout << "getAuth_password : " << _client_socket_v[i].getAuth_password() << std::endl;

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
				int buf_len = buf.size();
				std::cout << "[SERVER] : Commande complete\n";
				if (buf[buf_len - 1] == '\n' )
				{
					if (_client_socket_v[i].getAuth_password() == 0)
					{
						buf.erase(buf.size() - 1, 1);
						std::cout << buf << "|" << _password << "|" << std::endl;
						if (!buf.compare(_password))
						{

							_client_socket_v[i].setAuth_passwordOK();
							sendMessageSuccess(_client_socket_v[i].getFd(), "[SERVER] : authentication successful\n");
							std::cout << "good password" << std::endl;
						}
						else
						{
							sendMessageUnSuccess(_client_socket_v[i].getFd(), "[SERVER] : Wrong password\n");
						}
						_client_socket_v[i].clearBuf();
					}
					else
					{
						_client_socket_v[i].setBuf(buf);
						this->parsing_cmd( &_client_socket_v[i] );
					}
				}
				else
				{
					std::cout << "[SERVER] : add to buf\n";
					_client_socket_v[i].setBuf(buf);
				}

			}
		}
	}
}

void Server::parsing_cmd( User * user )
{
	std::string cmd;
	std::string rest;
	std::vector<std::string> v_parse;


	// for (size_t i = 0; i < user->getBuf().size(); i++)
	// 	std::cout << (int)user->getBuf()[i] << std::endl;
	// std::cout << "END buffer" << std::endl;

	v_parse = split_string(user->getBuf());

	// Affiche chaque élément du vecteur tokens
	// for (std::vector<std::string>::iterator it = v_parse.begin(); it != v_parse.end(); ++it) {
	// 	std::cout << *it << "|" << std::endl;
	// }

	if (v_parse[0][0] == '/')
	{
		std::cout << "[SERVER] : It's a commande: [" << v_parse[0] << "] !" << std::endl;
		this->handleCommandServer(v_parse[0], v_parse[1], *user);
		// user->handleCommand(v_parse[0], v_parse[1]);

	}

	user->clearBuf();
}

static Command option(const std::string& cmd)
{
	if (!cmd.compare("/NICK")) return (Nick);
	if (!cmd.compare("/JOIN")) return (Join);
	if (!cmd.compare("/NAMES")) return (Names);
	if (!cmd.compare("/LIST")) return (List);
	if (!cmd.compare("/WHOIS")) return (Whois);
	return (Unknown);
}

void	Server::handleCommandServer(std::string const& cmd, std::string const& rest, User& user)
{
	switch (option(cmd))
	{
		case Unknown:
			// throw std::string("Unknown Command !");
			break;
		case Nick:
			user.setNickname(rest);
			break;
		case Join:
			this->cmd_JoinChannel(rest, user);
			break;
		case Names:
			this->commandeServer_name(user);
			break;
		case List:
			this->cmd_DisplayChannel();
			break;
		case Whois:
			this->cmd_Whois(rest);
			break;
	}
}

bool	Server::channel_exist(std::string const& cnl_name)
{
	for (size_t i(0); i < this->_channel.size(); i++)
		if (!cnl_name.compare(this->_channel[i].getName()))
			return (true);
	return (false);
}


/*
** --------------------------------- COMMANDE ---------------------------------
*/

/*
 *	Command: JOIN
 *	Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
 *            / "0"
 *	----------
 *	The JOIN command is used by a user to request to start listening to
 *		the specific channel.
 *
 *	Once a user has joined a channel, he receives information about
 *		all commands his server receives affecting the channel.
 *
 *	If a JOIN is successful, the user receives a JOIN message
 *		as confirmation and is then sent the channel's topic
 *			and the list of users who are on the channel,
 *				which MUST include the user joining.
 *
 *	Note that this message accepts a special argument ("0"), which is
 *		a special request to leave all channels the user is currently a member of.
 *
 * 	----------
 * @example
 * 	/join &foobar		|	Command to join channel &foobar.
 *	/join &foo,&bar		|	Command to join channels &foo and &bar.
 *	/join 0				|	Leave all currently joined channels.
 *
 *	----------
 *	Si le rest est "0", l'User quitte tout les channels dont il fait parti
 *	Le rest est une premiere fois parser, renvoie un vector avec le ou les channels
 *	Si le nom du Canal n'est pas connu, il est cree
 *	Sinon l'User est ajoute au Canal
*/
void	Server::cmd_JoinChannel(std::string const& rest, User& user)
{
	if (!rest.compare("0"))
	{
		// command to quit all channel
		return ;
	}
	std::vector<std::string> cnl = parse_cnl_name(rest);

	for (size_t	i(0); i < cnl.size(); i++)
	{
		if (channel_exist(cnl[i])) // Verif que le channel existe dans le serveur
		{
			// std::cout << "there\n" ;
			for (size_t i(0); i < this->_channel.size(); i++)
				if (!cnl[i].compare(this->_channel[i].getName()))
					this->_channel[i].AddUser(user, false);
			// this->_channel[i].getUsers();
		}
		else // Le channel est cree
		{
			if (cnl[i][0] != '&' || cnl[i].size() > 50)
				break;
				// throw std::string("Invalid channel name !");
			Channel chan;
			chan.setName(rest);
			chan.AddUser(user, true);
			this->setNewChannel(chan);
			user.setListCnlMember(chan);
		}

	}
}

void	Server::commandeServer_name( const User & user )
{
	// std::cout << "commandeServer_name\n";
	for (size_t i = 0; i < _client_socket_v.size(); i++)
	{

		const std::string message = _client_socket_v[i].getNickname() + "\n";
		user.sendMessage(message);
	}
}

void	Server::cmd_DisplayChannel() const
{
	std::cout << "display servers" << std::endl;
	for (size_t i(0); i < this->_channel.size(); i++)
		std::cout << this->_channel[i].getName() << std::endl;
	std::cout << "---" << std::endl;
}

void	Server::cmd_Whois(std::string const& rest) const
{
	for (size_t i(0); i < this->_client_socket_v.size(); i++)
	{
		if (!rest.compare(this->_client_socket_v[i].getNickname()))
		{
			this->_client_socket_v[i].getListCnl();
			return ;
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int					Server::getServer_fd() const
{
	return this->_server_fd;
}
struct sockaddr_in	Server::getAddr() const
{
	return this->_addr;
}
int					Server::getPort() const
{
	return this->_port;
}

std::string			Server::getPassword() const
{
	return this->_password;
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

void	Server::setRmChannel(Channel& cnl)
{
	std::vector<Channel>::iterator it = this->_channel.begin();
	while (it != this->_channel.end())
	{
		if (!cnl.getName().compare(it->getName()))
			this->_channel.erase(it);
		it++;
	}
}

void	Server::setNewChannel(Channel& cnl)
{
	this->_channel.push_back(cnl);
}


/* ************************************************************************** */
