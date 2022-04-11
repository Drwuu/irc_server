/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 22:46:03 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/11 22:54:54 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>

struct Socket {
	typedef		int						fd_type;
	typedef		const int				const_fd_type;
	typedef		struct sockaddr_in6		address_type;
	typedef		const address_type		const_address_type;
	typedef		int						port_type;
	typedef		unsigned int			len_type;
	typedef		char	*				data_type;

	fd_type					sockfd;
	port_type				port;
	address_type			address;
	len_type				len;
};

#endif
