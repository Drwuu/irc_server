/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 15:36:31 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/16 20:25:13 by guhernan         ###   ########.fr       */
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
# include <sys/socket.h>

# include <string>
# include <unistd.h>
# include <iostream>

# include <poll.h>
# include <list>
# include <map>
# include <vector>


///////////////////////////////////////////////////////////////////////////////
//// Requestable events.  If poll(2) finds any of these set, they are
//// copied to revents on return.
// #define POLLIN          0x0001     //	  1		any readable data available		
// #define POLLPRI         0x0002     //	  2		OOB/Urgent readable data
// #define POLLOUT         0x0004     //	  4		file descriptor is writeable
// #define POLLRDNORM      0x0040     //	 40		non-OOB/URG data available
// #define POLLWRNORM      POLLOUT    //	  4		no write type differentiation
// #define POLLRDBAND      0x0080     //	128		OOB/Urgent readable data
// #define POLLWRBAND      0x0100     //	256		OOB/Urgent data can be written
//
//
//// FreeBSD extensions: polling on a regular file might return one
//// of these events (currently only supported on local filesystems).
// #define POLLEXTEND      0x0200     //	512		file may have been extended
// #define POLLATTRIB      0x0400     //	1024	file attributes may have changed
// #define POLLNLINK       0x0800     //	2048	(un)link/rename may have happened
// #define POLLWRITE       0x1000     //	4096	file's contents may have changed
//
//
//// These events are set if they occur regardless of whether they were
//// requested.
// #define POLLERR         0x0008     //		8	some poll error occurred
// #define POLLHUP         0x0010     //		16	file descriptor was "hung up"
// #define POLLNVAL        0x0020     //		32	requested events "invalid"
//
// #define POLLSTANDARD    (POLLIN|POLLPRI|POLLOUT|POLLRDNORM|POLLRDBAND|\
					 // POLLWRBAND|POLLERR|POLLHUP|POLLNVAL)


///////////////////////////////////////////////////////////////////////////////
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
		typedef		Socket<Address_ipv6>		socket_type;
		typedef		socket_type::port_type		port_type;
		typedef		socket_type::fd_type		fd_type;
		typedef		socket_type::data_type		data_type;
		typedef		short int					flag_type;
		typedef		int							milisecond_type;

	private:
		class	IPoll_handling {

			protected:
				IPoll_handling();
				Proxy	*_proxy;
			public:
				IPoll_handling(Proxy *proxy);
				virtual ~IPoll_handling();
				virtual void	handle(socket_type *socket) = 0;
				virtual void	handle_server(socket_type *socket) = 0;
		};

		class Poll_in : public IPoll_handling {
			public:
				Poll_in();
				~Poll_in();
				Poll_in(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

		class Poll_priority_in : public IPoll_handling {
			public:
				Poll_priority_in();
				~Poll_priority_in();
				Poll_priority_in(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

		class Poll_invalid : public IPoll_handling {
			public:
				Poll_invalid();
				~Poll_invalid();
				Poll_invalid(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

		class Poll_hang_up : public IPoll_handling {
			public:
				Poll_hang_up();
				~Poll_hang_up();
				Poll_hang_up(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

		class Poll_error : public IPoll_handling {
			public:
				Poll_error();
				~Poll_error();
				Poll_error(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

		class Poll_out : public IPoll_handling {
			public:
				Poll_out();
				~Poll_out();
				Poll_out(Proxy *proxy);
				void	handle(socket_type *socket);
				void	handle_server(socket_type *server_socket);
		};

	public:
		typedef		std::list<data_type>						cache_queue_type;

		typedef		std::map<fd_type, socket_type *>			client_tree_type;
		typedef		std::map<fd_type, cache_queue_type >		cache_tree_type;
		typedef		std::vector<pollfd>							pollfd_type;

		typedef		std::map<flag_type, IPoll_handling *>		flag_tree_type;

		typedef		Socket_event								event_type;
		typedef		std::list<event_type *>						api_type;

	private:

		// Server socket is isolated from clients.
		socket_type				_server;

		// Tree to find and use : connected clients, flags actions, pending messages.
		client_tree_type		_clients;
		cache_tree_type			_cache;
		flag_tree_type			_flags;

		// Poll() arguments : pollfd *, timeout.
		pollfd_type				_poll_data;
		milisecond_type			_timeout;

		// List destined to the server.
		api_type				_to_server;

		// There is only 3 sets of events :
		// 		Client base : POLLIN | POLLPRI (read)
		// 		Client message : POLLIN | POLLPRI | POLLOUT (read | write)
		// 		Server : POLLIN (listen connexion)
		// Constructors' initialise.
		// Server one's should never change but the dev can implement it.
		// Every pollfd.events is acutalised at each poll() loop.
		short					_cl_base_pevents;
		short					_cl_message_pevents;
		short					_sv_pevents;

		short					_cl_hang_up;
		short					_cl_invalid;
		short					_cl_error;
	
	private:
		// Should stay unaccessible
		Proxy();

		void		init_flags();
		void		init_server_socket();
		void		init_poll_events();

		void		set_flags();
		void		clear_flags();

		void		add_client(const socket_type &new_client);
		void		insert_client(const socket_type &new_client);
		void		insert_empty_cache(const socket_type &new_client);
		void		insert_pollfd(const socket_type &new_client);

		void		delete_client(const socket_type &target);
		void		erase_cache(const socket_type &target);
		void		erase_pollfd(const socket_type &target);
		void		erase_client_socket(const socket_type &target);

		int			receive(const socket_type *client);
		int			send_to_client(const socket_type *client, const data_type data);

		void		clean_api();

	public:
		Proxy(const port_type &port);
		Proxy(const Proxy &source);
		~Proxy();

		Proxy	&operator=(const Proxy &source);

		void		end_all_connexions();
		// Usefull to Server class : Proxy gives sockets to the Server (among other things)
		void		end_connexion(socket_type &target);

		void		switch_on();
		void		switch_off();

		void		set_timeout(milisecond_type timeout);

		// main poll() loop
		void		queuing();

		api_type	send_api();
		void		receive_api(api_type &data);
};

#endif
