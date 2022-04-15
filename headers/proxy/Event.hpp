/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 22:43:30 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/15 14:39:39 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
# define EVENT_HPP

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
			data_type		data;
		public:
			void			handle();
	};

	class	Write_priority : public Socket_event {
		private:
			// Message data to send to Client
			data_type		data;
		public:
			void			handle();
	};

	class	Confirm_connexion : public Socket_event {
		private:
			// Confirmation message for the Client
			data_type		data;
		public:
			void			handle();
	};

	class	Refuse_connexion : public Socket_event {
		private:
			// Refuse message for the Client
			data_type		data;
		public:
			void			handle();
	};

	class	Disconnect_all : public Socket_event {
		private:
			// Disconnection message for all Clients
			data_type		data;
		public:
			void			handle();
	};

	class	Disconnect : public Socket_event {
			// Disconnection message for the Client
		private:
			data_type		data;
		public:
			void			handle();
	};

	class	Signal : public Socket_event {
		private:
			// Signal response for the Client
			data_type		data;
		public:
			void			handle();
	};
};


///////////////////////////////////////////////////////////////////////////////////
// SERVER HANDLING
//
// Inherited methods. These will be instanciated by the Proxy class, written in the 
// Server class, used in the Server class.
// This is a unified API form thanks to polymorphisme
// All the server has to do, is receive data and run handle() on a for loop.


struct Server_queue {

	class	Message : public Socket_event {
		private:
			// Content of the message from the Client
			data_type			data;
			const socket_type *	socket;
		public:
			~Message();
			Message(data_type data, const socket_type *socket);
			void			handle();
	};

	class	Message_priority : public Socket_event {
		private:
			// Content of the priority message from the Client
			data_type		data;
		public:
			void			handle();
	};

	class	Request_connexion : public Socket_event {
		private:
			// Request details from the Client
			data_type		data;
		public:
			Request_connexion();
			~Request_connexion();
			void			handle();
	};

	class	Client_disconnected : public Socket_event {
		// Inform the server of a disconnection
		public:
			void			handle();
	};

	class	Error : public Socket_event {
		// Inform the server of an error on sockets
		private:
			// Data took from the error (if needed)
			data_type		data;
		public:
			void			handle();
	};

	class	Signal : public Socket_event {
		private:
			// Signal receive from the Client
			data_type		data;
		public:
			void			handle();
	};

};

#endif
