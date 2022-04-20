/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy_queue.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwourms <lwourms@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 22:43:30 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 19:58:58 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROXY_QUEUE_HPP
# define PROXY_QUEUE_HPP

# include "Socket_event.hpp"

///////////////////////////////////////////////////////////////////////////////////
// PROXY HANDLING
//
// Inherited methods. These will be instanciated by the Server class, written in the
// Proxy class, used in the Proxy class. Feel free to add more polymorphique class.
// This is a unified API form thanks to polymorphisme.
// All the Proxy has to do, is receive data and run handle() on a for loop.
//

namespace irc {

	class Proxy;

	struct Proxy_queue {
		class	Write : public Socket_event {
			private:
				// Message to sent to Client
				data_type		_data;
				fd_type			_socketfd;
				void			handle();
				Write();
			public:
				Write(fd_type socketfd, data_type data);
				~Write();
				void			handle(Proxy &);
		};

		class	Write_priority : public Socket_event {
			private:
				// Message data to send to Client
				data_type		_data;
				fd_type			_socketfd;
				void			handle();
				Write_priority();
			public:
				Write_priority(fd_type socketfd, data_type data);
				~Write_priority();
				void			handle(Proxy &);
		};

		class	Disconnect_all : public Socket_event {
			private:
				// Disconnection message for all Clients
				// data_type		data;
				void			handle();
			public:
				Disconnect_all();
				~Disconnect_all();
				void			handle(Proxy &);
		};

		class	Disconnect : public Socket_event {
			// Disconnection message for the Client
			private:
				fd_type			_socket_id;
				Disconnect();
				void			handle();
			public:
				Disconnect(const fd_type &socket_id);
				~Disconnect();
				void			handle(Proxy &);
		};
	};
}

# include "Proxy.hpp"

#endif
