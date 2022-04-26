/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 15:56:39 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/26 16:40:09 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <unistd.h>
# include <stdlib.h>

# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include "Address.hpp"

# include <iostream>

# define TCP_PROTOCOL 6


template <typename AddSock>
class Socket {
	public:
		typedef		int											fd_type;
		typedef		int											protocol_type;
		typedef		int											socket_sort;
		typedef		const int									const_fd_type;
		typedef		const char	*								data_type;
		typedef		unsigned int								len_type;

		typedef		AddSock										address_type;
		typedef		typename address_type::value_type			address_value_type;
		typedef		typename address_type::address_type			ipv6_type;
		typedef		typename address_type::len_type				addrlen_type;
		typedef		typename address_type::port_type			port_type;


	private:
		// len && port in _address
		fd_type					_sockfd; // fd as communication point
		socket_sort				_type; // STREAM_SOCK or others
		protocol_type			_protocol; // 0 to let the machine decide
		address_type			_address; //address IP, port. length...

	protected:
		Socket()
			: _sockfd(0), _type(0), _protocol(0), _address(0) { }

	public:
		// Constructor destined to create Server Sockets.
		Socket(const port_type &port, const socket_sort &type, const protocol_type &protocol = 0)
			: _sockfd(0), _type(type), _protocol(protocol), _address(port) { }

		virtual ~Socket() { }

		Socket(const Socket &source)
			: _sockfd(source._sockfd), _type(source._type),
			_protocol(source._protocol), _address(source._address) { }
		Socket	&operator=(const Socket &source) {
			_sockfd = source._sockfd;
			_type = source._type;
			_protocol = source._protocol;
			_address = source._address;
			return *this;
		}

		fd_type				get_fd() const { return _sockfd; }
		addrlen_type		get_addrlen() const { return _address.get_len(); }
		port_type			get_port() const { return _address.get_port(); }
		std::string			get_address_readable() const { return _address.get_readable_address(); }
		socket_sort			get_socket_sort() const { return _type; };
		address_value_type	get_address_data() const { return _address.data(); };


		void			create_endpoint() {
			std::clog << " ---- Create endpoint on port " << this->get_port() << " ... " <<  std::endl;
			_sockfd = socket(_address.get_family(), _type, _protocol);
			fcntl(_sockfd, F_SETFL, O_NONBLOCK);
			if (_sockfd == -1) {
				std::cerr << " [ERROR] : socket function failed -- " << strerror(errno) << std::endl;
				return ;
			}
			int		opt_val = true;
			// define TCP_PROTOCOL 6 (netinet/in.h)
			if (setsockopt(_sockfd, TCP_PROTOCOL, SO_REUSEADDR, &opt_val, sizeof(int)) == -1) {
				std::cerr << " [ERROR] : adding socket option failed -- [SO_REUSEADDR] " << strerror(errno) << std::endl;
				return ;
			}
			std::clog << " ---- Endpoint created on the address " << "" << std::endl;
		}

		void			end_connexion() {
			if (_sockfd != 0)
				if (close(_sockfd) == -1)
					std::clog << " [ERROR] closing socket on fd " << _sockfd
						<< " using port " << get_port() << std::endl;
			_sockfd = 0;
		}

		void			renew(const port_type &port, const socket_sort &type, const protocol_type &protocol = 0) {
			this->end_connexion();
			*this = Socket(port, type, protocol);
		}

		void			bind_socket() {
			std::clog << " ---- Binding socket on port " << this->get_port() << " ... " <<  std::endl;
			while (bind(_sockfd, (sockaddr *)&(*_address), _address.get_len()) == -1)
				sleep(1);
			std::clog << " ---- Binding done." <<  std::endl;
		}

		// max_queue defines the maximum length for the queue of pending connections. (man length)
		void			listen_for_connexion(int max_queue = 5) {
			std::clog << " -- Make socket listen on port " << this->get_port() << " ... " <<  std::endl;
			if (listen(_sockfd, max_queue) == -1) {
				std::cerr << "[ERROR] listen failed " << std::endl;
				return ;
			}
			std::clog << " ---- The socket is listening on port " << this->get_port() << "." <<  std::endl;
		}

		Socket			accept_connexion() {
			std::clog << " ---> Accepting connexion on Server [" << this->get_address_readable()
				<< "] -- identified by the fd [" << this->_sockfd
				<< "] -- bind to port [" << this->get_port() << "] ..." << std::endl;
			Socket			new_client;
			len_type		new_len;

			new_client._sockfd = accept(_sockfd, (sockaddr *)&(*new_client._address), &new_len);
			new_client._address.set_len(new_len);
			new_client._address.set_port(this->get_port());
			new_client._address.set_family(this->_address.get_family());
			new_client._type = this->_type;
			new_client._protocol = this->_protocol;

			if (new_client.get_fd() == -1) {
				std::cerr << " [ERROR] accept failed " << std::endl;
			}
			std::clog << " -> Connexion succed with Client [" << new_client.get_address_readable()
				<< "] - identified by the fd [" << new_client.get_fd() << "] " << std::endl;
			return new_client;
		}

		template <typename T>
			friend bool		operator==(const Socket<T> &rhs, const Socket<T> &lhs) {
				return rhs._sockfd == lhs._sockfd && rhs._type == lhs.fd_type
					&& rhs._address == lhs._address && rhs._protocol == lhs._protocol;
			}

		template <typename T>
			friend bool		operator!=(const Socket<T> &rhs, const Socket<T> &lhs) {
				return !(rhs == lhs);
			}

};


#endif
