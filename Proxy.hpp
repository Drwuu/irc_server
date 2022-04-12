/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 15:36:31 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/12 23:11:39 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROXY_HPP
# define PROXY_HPP

# include "Socket.hpp"
# include "Event.hpp"
# include "Address.hpp"

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

class	Poll_handling {
};

class	Proxy {

	// Typedef
	public:
		typedef		Socket<Address_ipv6>				socket_type;
		typedef		socket_type::port_type				port_type;
		typedef		socket_type::fd_type				fd_type;
		typedef		short int							flag_type;

		typedef		std::map<fd_type, socket_type>		client_tree_type;
		typedef		std::map<flag_type, Poll_handling>	flag_tree_type;
		typedef		Socket_event						event_type;
		typedef		std::list<event_type>				api_type;

	private:
		socket_type				_server;
		client_tree_type		_clients;
		flag_tree_type			_flags;
		api_type				_to_server;
		api_type				_to_client;
	
	private:
		// Should stay unaccessible
		Proxy();
		void		create_socket(socket_type &source);
		void		generate_address();
		void		bind_socket();
		void		listen();

		void		manage_new_connexion(socket_type &new_client);
	
	public:
		Proxy(const port_type &port);
		Proxy(const Proxy &source);
		~Proxy();

		Proxy	&operator=(const Proxy &source);

		void		switch_on();
		void		switch_off();

		// main loop
		int			queuing();

		api_type	send_data();
		void		receive_data(const api_type &data);
};

#endif
