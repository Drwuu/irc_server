/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_event.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:43:59 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 19:39:06 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_EVENT_HPP
# define SOCKET_EVENT_HPP

# include "Socket.hpp"

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
			virtual void	handle() = 0;
	};


#endif
