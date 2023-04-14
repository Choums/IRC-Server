/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:44:15 by aptive            #+#    #+#             */
/*   Updated: 2023/04/14 17:15:56 by aptive           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "web_serv.hpp"

class User
{
	public:
				User(/* args */);
				User(User const & src);
				User(int fd);
		virtual	~User();

		User & operator=(User const & rhs);
		// ** --------------------------------- METHODS ----------------------------------
		void	handleCommand(const std::string& cmd, const std::string& rest);
		void	sendMessage(const std::string& message);


		// ** --------------------------------- ACCESSOR ---------------------------------
		int		getFd(void) const;
		int		getId(void) const;
		std::string		getNickname(void) const;

		// ** --------------------------------- SETTER ---------------------------------
		void	setNickname(const std::string& rest);

	private:
		int _fd;
		int _id;
		std::string _nickname;
};

std::ostream &			operator<<( std::ostream & o, User const & i );


void gestion_new_connexion(int server_fd, fd_set * temp, int * max_socket_fd, fd_set * read_sockets, struct sockaddr_in addr, std::vector<User> * client_socket_v);
void gestion_activite_client(fd_set * read_sockets, std::vector<User> * client_socket_v,fd_set * temp);
void parsing_cmd(std::string buffer, User * user);

#endif /* ****************************************************** USER_H */
