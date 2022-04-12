/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 15:56:39 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/12 23:03:24 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include "unistd.h"
# include <sys/socket.h>
# include <netinet/in.h>
# include "Address.hpp"

# include <iostream>


template <typename AddSock>
class Socket {
	public:
		typedef		int											fd_type;
		typedef		int											protocol_type;
		typedef		int											socket_sort;
		typedef		const int									const_fd_type;
		typedef		char	*									data_type;

		typedef		AddSock										address_type;
		typedef		typename address_type::address_type			ipv6_type;
		typedef		typename address_type::len_type				len_type;
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

		Socket(const Socket &source) { *this = source; }
		Socket	&operator=(const Socket &source) {
			_sockfd = source._sockfd;
			_type = source._type;
			_protocol = source._protocol;
			_address = source._address;
		}

		fd_type			get_fd() { return _sockfd; }
		len_type		get_len() { return _address.get_len(); }
		port_type		get_port() { return _address.get_port(); }
		std::string		get_address_readable() { return _address.get_readable_address(); }
		socket_sort		get_socket_sort() { return _type; };

		void			create_endpoint() {
			std::clog << "Create endpoint on port " << this->get_port() << " ... " <<  std::endl;
			_sockfd = socket(_address.get_family(), _type, _protocol);
			if (_sockfd == -1) {
				std::cerr << " error : socket function failed." << std::endl;
				return ;
			}
		}
		void			end_connexion() {
			if (_sockfd != 0)
				if (close(_sockfd) == -1)
					std::clog << " [ERROR] closing socket on fd " << _sockfd
							<< " using port " << get_port() << std::endl;
			_sockfd = 0;
		}

		void			renew(const port_type &port, const socket_sort &type, const protocol_type &protocol = 0) {
			end_connexion();
			*this = Socket(port, type, protocol);
		}

		void			bind_socket() {
			std::clog << "Binding socket on port " << this->get_port() << " ... " <<  std::endl;
			if (bind(_sockfd, (sockaddr *)*_address, _address.get_len()) == -1) {
				std::cerr << " [ERROR] bind_socket failed " << std::endl;
			}
			std::clog << "Binding done." <<  std::endl;
		}

		// max_queue defines the maximum length for the queue of pending connections. (man length)
		void			listen_for_connexion(int max_queue) {
			std::clog << "Make socket listen on port " << this->get_port() << " ... " <<  std::endl;
			if (listen(_sockfd, 5) == -1) {
				std::cerr << "[ERROR] listen failed " << std::endl;
			}
		}

		Socket			accept_connexion() {
			std::clog << "Accepting connexion on fd " << this->_sockfd
				<< " bind to port " << this->get_port() << " ..." << std::endl;
			Socket			new_client;
			len_type		new_len;
			new_client._sockfd = accept(_sockfd, (sockaddr *)*new_client._address, &new_len);
			new_client._address.set_len(new_len);
			new_client._address.set_port(this->get_port());
			new_client._address.set_family(this->_address.get_family());
			new_client._type = this->_type;
			new_client._protocol = this->_protocol;
			if (new_client.get_fd() == -1) {
				std::cerr << " [ERROR] accept failed " << std::endl;
			}
			return new_client;
		}

};

#endif
