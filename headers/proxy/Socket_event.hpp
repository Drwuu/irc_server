/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_event.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:43:59 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/20 14:11:29 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_EVENT_HPP
# define SOCKET_EVENT_HPP

# include "Socket.hpp"

namespace irc {

	class	Proxy;
	class	Server;

	class	Socket_event {
		public:
			typedef		Socket<Address_ipv6>	socket_type;
			typedef		socket_type::fd_type	fd_type;
			typedef		socket_type::data_type	data_type;

		protected:
			fd_type			sockfd;

		public:
			Socket_event();
			virtual ~Socket_event();
			virtual void	handle(irc::Proxy &proxy) = 0;
			virtual void	handle(irc::Server &server) = 0;
	};

}

#endif
