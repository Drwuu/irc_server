/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 15:36:31 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/11 23:35:16 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROXY_HPP
# define PROXY_HPP

# include "Socket.hpp"
# include "Event.hpp"

# include <sys/types.h>
# include <sys/time.h>
# include <sys/event.h>

# include <string>
# include <unistd.h>
# include <iostream>

# include <poll.h>
# include <list>
# include <map>

// struct sockaddr_in6 {
// 		__uint8_t       sin6_len;       [> length of this struct(sa_family_t) <]
// 		sa_family_t     sin6_family;    [> AF_INET6 (sa_family_t) <]
// 		in_port_t       sin6_port;      [> Transport layer port # (in_port_t) <]
// 		__uint32_t      sin6_flowinfo;  [> IP6 flow information <]
// 		struct in6_addr sin6_addr;      [> IP6 address <]
// 		__uint32_t      sin6_scope_id;  [> scope zone index <]
// };

// struct pollfd {
//     int    fd;       /* file descriptor */
//     short  events;   /* events to look for */
//     short  revents;  /* events returned */
// };

// FIXME : manage Signal Handling ?


class	Proxy {

	// Typedef
	public:
		typedef		Socket								socket_type;
		typedef		socket_type::port_type				port_type;
		typedef		socket_type::fd_type				fd_type;

		typedef		std::map<fd_type, socket_type>		client_socket;
		typedef		Socket_event						event_type;
		typedef		std::list<event_type>				api_type;

	private:
		socket_type		server;
		socket_type		*client;
		api_type		send_packet;
		api_type		receive_packet;
	
	private:
		Proxy();
		void		create_socket();
		void		generate_address();
		void		bind_socket();
		void		listen();
	
	public:
		Proxy(const port_type &port);
		~Proxy();

		void		switch_on();
		void		switch_off();

		// main loop
		int			queuing();

		api_type	send_data();
		void		receive_data(const api_type &data);
};

#endif
