/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaidel <chaidel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:44:15 by aptive            #+#    #+#             */
/*   Updated: 2023/05/30 18:10:10 by chaidel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "ft_irc.hpp"

class Channel;

class User
{
	public:
				User(/* args */);
				User(User const & src);
				User(int fd);
		virtual	~User();

		User & operator=(User const & rhs);
		// ** --------------------------------- METHODS ----------------------------------
		// void	handleCommand(const std::string& cmd, const std::string& rest);
		void	sendMessage(const std::string& message) const;
		void	clearBuf( void );
		bool	is_set();

		// ** --------------------------------- ACCESSOR ---------------------------------
		std::string				getNames() const;
		bool					getSet() const;
		int						getFd(void) const;
		int						getId(void) const;
		std::string				getUsername() const;
		std::string				getHostname() const;
		std::string				getNickname(void) const;
		std::string				getBuf(void) const;
		std::string				getModes() const;
		bool					getOper(void) const;
		bool					getInv() const;
		bool					getAuth_password(void) const;
		std::vector<Channel*>	getListCnl();
		bool					Is_PassSet();
		// ** --------------------------------- SETTER ---------------------------------

		void	setSetUser();
		void	setUserMode(std::string const& mode);
		void	setUsername(std::string const& name);
		void	setHostname(std::string const& host);
		void	setNickname(const std::string& rest);
		void	setBuf(const std::string& buf);
		void	setAdmin(const bool & admin);
		void	setAuth_passwordOK( void );
		void	setRmCnlMembership(Channel* cnl);
		void	setAddListCnlMember(Channel* cnl);

	private:
		bool					_set;
		int						_fd;
		int						_id;
		std::string				_username; // L'user sera set une fois que les cmd User, Nick soient appelées
		std::string				_nickname;
		std::string				_hostname;
		std::string				_buf;
		bool					_oper;
		bool					_inv;
		bool					_auth_password;
		std::vector<Channel*>	_list_cnl;
};

std::ostream &			operator<<( std::ostream & o, User const & i );


void gestion_new_connexion(int server_fd, fd_set * temp, int * max_socket_fd, fd_set * read_sockets, struct sockaddr_in addr, std::vector<User> * client_socket_v);
void gestion_activite_client(fd_set * read_sockets, std::vector<User> * client_socket_v,fd_set * temp);
void parsing_cmd(std::string buffer, User * user);
#endif /* ****************************************************** USER_H */
