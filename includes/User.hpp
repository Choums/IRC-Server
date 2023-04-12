/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aptive <aptive@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:44:15 by aptive            #+#    #+#             */
/*   Updated: 2023/04/12 19:06:04 by aptive           ###   ########.fr       */
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

		// ** --------------------------------- ACCESSOR ---------------------------------
		int		getFd(void) const;

	private:
		int _fd;
};

std::ostream &			operator<<( std::ostream & o, User const & i );


#endif /* ****************************************************** USER_H */
