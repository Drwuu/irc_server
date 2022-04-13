/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 15:36:31 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/13 18:40:09 by guhernan         ###   ########.fr       */
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
# include <vector>

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
	
	public:
		typedef		Socket<Address_ipv6>				socket_type;
		typedef		socket_type::port_type				port_type;
		typedef		socket_type::fd_type				fd_type;
		typedef		short int							flag_type;

	private:
		class	Poll_handling {

			Poll_handling();
			~Poll_handling();

		};

		class Poll_in : public Poll_handling {
			public:
				Poll_in();
				~Poll_in();
		};

		class Poll_priority_in : public Poll_handling {
			public:
				Poll_priority_in();
				~Poll_priority_in();
		};

		class Poll_invalid : public Poll_handling {
			public:
				Poll_invalid();
				~Poll_invalid();
		};

		class Poll_hang_up : public Poll_handling {
			public:
				Poll_hang_up();
				~Poll_hang_up();
		};

		class Poll_error : public Poll_handling {
			public:
				Poll_error();
				~Poll_error();
		};

		struct Pollfd {
		};

	public:
		typedef		std::map<fd_type, socket_type>			client_tree_type;
		typedef		std::map<flag_type, Poll_handling *>	flag_tree_type;
		typedef		Socket_event							event_type;
		typedef		std::list<event_type>					api_type;
		typedef		std::vector<struct pollfd>				poll_type;
		typedef		int										milisecond_type;

	private:
		socket_type				_server;
		client_tree_type		_clients;
		flag_tree_type			_flags;
		poll_type				_poll_data;
		milisecond_type			_timeout;
		api_type				_to_server;
		api_type				_to_clients;

	
	private:
		// Should stay unaccessible
		Proxy();

		void		end_all_connexions();
		void		end_connexion();

		void		init_flags();
		void		init_server_socket();
		void		init_poll_args();

		void		update_flags();

	public:
		Proxy(const port_type &port);
		Proxy(const Proxy &source);
		~Proxy();

		Proxy	&operator=(const Proxy &source);

		void		switch_on();
		void		switch_off();

		void		set_timeout(milisecond_type timeout);

		// main poll() loop
		void		queuing();

		api_type	send_data();
		void		receive_data(const api_type &data);
};

#endif
