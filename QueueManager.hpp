/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QueueManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 15:36:31 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/09 18:41:54 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUEMANAGER_HPP
# define QUEUEMANAGER_HPP

# include <sys/types.h>
# include <sys/time.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <netinet/in.h>

# include <string>
# include <unistd.h>
# include <iostream>

	//// struct kevent :
	// uintptr_t       ident;          [> identifier for this event <]
	// int16_t         filter;         [> filter for event <]
	// uint16_t        flags;          [> general flags <]
	// uint32_t        fflags;         [> filter-specific flags <]
	// intptr_t        data;           [> filter-specific data <]
	// void            *udata;         [> opaque user data identifier <]

	// struct sockaddr_in6 {
		// __uint8_t       sin6_len;       [> length of this struct(sa_family_t) <]
		// sa_family_t     sin6_family;    [> AF_INET6 (sa_family_t) <]
		// in_port_t       sin6_port;      [> Transport layer port # (in_port_t) <]
		// __uint32_t      sin6_flowinfo;  [> IP6 flow information <]
		// struct in6_addr sin6_addr;      [> IP6 address <]
		// __uint32_t      sin6_scope_id;  [> scope zone index <]
	// };

	// FIXME : add filter for Signal Handling ?

struct	Filters {
};

class	Events {
	public:
		typedef		struct	kevent	value_type;
		typedef		value_type	*	pointer;
		typedef		int				fd_type;
		typedef		const int &		const_reference_fd;

	private:
		pointer		_list;
		fd_type		_sockfd;

	public:
		Events(const int &sockfd);
		~Events();

		void		add_filter();
		void		remove_filter();
		void		disable_filter();
		void		enable_filter();
};

class	User {

	public:
		class Client {
			public:
				typedef		int						fd_type;
				typedef		struct sockaddr_in6		address_type;
				typedef		const address_type		const_address_type;
				typedef		int						port_type;
				typedef		Events					event_type;
			private:
				fd_type					_sockfd;
				address_type			_address;
				unsigned int			_len;
				event_type				_events;
				Client();
			public:
				Client(const_address_type  address);
				~Client();
				void				set_sockfd(int sockfd);

				const address_type	&get_address();

				int			add_write();
				int			add_read();
				int			remove_write();
				int			remove_read();

				int			enable_write();
				int			enable_read();
				int			disable_write();
				int			disable_read();
		};

};

class	QueueManager {

	public :
		typedef		User::Client		client_type;
		typedef		User::Client	&	client_reference;

	private :
		int				_kq;
	
	public :
		QueueManager();
		~QueueManager();
};

#endif
