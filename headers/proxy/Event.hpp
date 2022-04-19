/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwourms <lwourms@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 22:43:30 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/19 17:23:26 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
# define EVENT_HPP

# include "Socket.hpp"

//// Double inclusion patch :
// Event.hpp already included in Server.hpp
class	Server;
// Event.hpp already included in Proxy.hpp
class	Proxy;

///////////////////////////////////////////////////////////////////////////////////
// Abstract class

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

///////////////////////////////////////////////////////////////////////////////////
// PROXY HANDLING
//
// Inherited methods. These will be instanciated by the Server class, written in the
// Proxy class, used in the Proxy class. Feel free to add more polymorphique class.
// This is a unified API form thanks to polymorphisme.
// All the Proxy has to do, is receive data and run handle() on a for loop.
//
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


///////////////////////////////////////////////////////////////////////////////////
// SERVER HANDLING
//
// Inherited methods. These will be instanciated by the Proxy class, written here,
// used in the Server class.
// This is a unified API form thanks to polymorphisme
// All the Server has to do, is receive data and run handle() on a for loop.

struct Server_queue {
	class	Message : public Socket_event {
		private:
			// Content of the message from the Client
			data_type			_data;
			const socket_type *_socket;
			Message();
			void			handle();
		public:
			~Message();
			Message(data_type data, const socket_type *client);
			void			handle(Server &server);
	};

	class	Message_priority : public Socket_event {
		private:
			// Content of the priority message from the Client
			data_type		_data;
			const socket_type *_socket;
			Message_priority();
			void			handle();
		public:
			~Message_priority();
			void			handle(Server &server);
	};

	class	Request_connexion : public Socket_event {
		private:
			// Request details from the Client
			const socket_type	*_socket;
			Request_connexion();
			void			handle();
		public:
			Request_connexion(const socket_type *client);
			~Request_connexion();
			void			handle(Server &server);
	};

	class	Client_disconnected : public Socket_event {
		// Inform the Server of a disconnection
			const socket_type	*_socket;
			Client_disconnected();
			void			handle();
		public:
			Client_disconnected(const socket_type *socket);
			~Client_disconnected();
			void			handle(Server &server);
	};

	class	Error : public Socket_event {
		// Inform the Server of an error on sockets
		private:
			// Data took from the error (if needed)
			data_type		_data;
			const socket_type	*_socket;
			void			handle();
		public:
			Error();
			~Error();
			void			handle(Server &server);
	};

	class	Signal : public Socket_event {
		private:
			// Signal receive from the Client
			data_type		_data;
			const socket_type	*_socket;
			void			handle();
		public:
			Signal();
			~Signal();
			void			handle(Server &server);
	};

};

# include "../structure/Server.hpp"
#endif
