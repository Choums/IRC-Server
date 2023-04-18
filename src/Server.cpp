/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:37:42 by aptive            #+#    #+#             */
/*   Updated: 2023/04/18 12:50:59 by aptive           ###   ########.fr       */
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
	std::cout << "Configuration server en cours ..." << std::endl;

	this->_port = port;
	this->_password = password;

	this->creation_socket_server(&this->_server_fd);
	this->configuration_socket_server(&this->_server_fd);
	this->creation_address_connexion(port, &this->_addr);
	this->association_socket_to_address(&this->_server_fd, &this->_addr);
	this->mode_listing_socket(&this->_server_fd);

	std::cout << "Configuration server succesful" << std::endl;

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
	int server_fd = this->getServer_fd();
	struct sockaddr_in addr = this->getAddr();

	// Initialize the read sockets set
	fd_set read_sockets;
	FD_ZERO(&read_sockets);
	FD_SET(server_fd, &read_sockets);

	int max_socket_fd = server_fd;

	// Boucle principale du serveur
	int count(0);
	while (true)
	{
		std::cout << "en attende de donnees : " << YELLOW << count << END << std::endl;
		count++;
		if (count > 20)
			exit(EXIT_SUCCESS);

		// Wait for any of the sockets to become readable
		fd_set temp = read_sockets;
		if (select(max_socket_fd + 1, &temp, NULL, NULL, NULL) < 0)
			throw std::string("Error : waiting for sockets to become readable !\n");


		// traitement de l'activité sur les sockets
		gestion_new_connexion(server_fd, &temp, &max_socket_fd, &read_sockets, addr, &client_socket_v);

		affichage_vector(client_socket_v);

		// Check for data on any of the client sockets
		gestion_activite_client( &read_sockets, &client_socket_v, &temp );

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




/* ************************************************************************** */
