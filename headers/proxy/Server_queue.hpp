/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_queue.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:46:28 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/20 14:07:53 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_QUEUE_HPP
# define SERVER_QUEUE_HPP

# include "Socket_event.hpp"

///////////////////////////////////////////////////////////////////////////////////
// SERVER HANDLING
//
// Inherited methods. These will be instanciated by the Proxy class, written here,
// used in the Server class.
// This is a unified API form thanks to polymorphisme
// All the Server has to do, is receive data and run handle() on a for loop.

namespace irc {

	class Server;

	struct Server_queue {
		class	Message : public Socket_event {
			private:
				// Content of the message from the Client
				data_type			_data;
				const socket_type *_socket;
				Message();
				void			handle(irc::Proxy &proxy);
			public:
				~Message();
				Message(data_type data, const socket_type *client);
				void			handle(irc::Server &server);
		};

		class	Request_connexion : public Socket_event {
			private:
				// Request details from the Client
				const socket_type	*_socket;
				Request_connexion();
				void			handle(irc::Proxy &proxy);
			public:
				Request_connexion(const socket_type *client);
				~Request_connexion();
				void			handle(irc::Server &server);
		};

		class	Client_disconnected : public Socket_event {
			// Inform the irc::Server of a disconnection
			const socket_type	*_socket;
			Client_disconnected();
			void			handle(irc::Proxy &proxy);
			public:
			Client_disconnected(const socket_type *socket);
			~Client_disconnected();
			void			handle(irc::Server &server);
		};

		class	Error : public Socket_event {
			// Inform the irc::Server of an error on sockets
			private:
				// Data took from the error (if needed)
				data_type		_data;
				const socket_type	*_socket;
				void			handle(irc::Proxy &proxy);
			public:
				Error();
				~Error();
				void			handle(irc::Server &server);
		};

	};
}

# include "../structure/Server.hpp"

#endif
