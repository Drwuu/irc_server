/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 23:03:17 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/15 17:46:48 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Proxy.hpp"

// Should stay unaccessible
Proxy::Proxy()
	: _server(0, SOCK_STREAM), _clients(), _cache(), _flags(), _poll_data(),
	_timeout(), _to_server(),
	_cl_base_pevents(), _cl_message_pevents(), _sv_pevents() { }

Proxy::Proxy(const port_type &port)
	: _server(port, SOCK_STREAM), _clients(), _cache(), _flags(), _poll_data(),
	_timeout(), _to_server(),
	_cl_base_pevents(), _cl_message_pevents(), _sv_pevents() {
		this->init_flags();
		this->init_poll_events();
	}

Proxy::Proxy(const Proxy &source)
	: _server(source._server), _clients(source._clients), _cache(source._cache),
	_flags(source._flags), _poll_data(source._poll_data),
	_timeout(source._timeout),  _to_server(source._to_server),
	_cl_base_pevents(source._cl_base_pevents), _cl_message_pevents(source._cl_message_pevents),
	_sv_pevents(source._sv_pevents) { }

Proxy::~Proxy() {
	this->switch_off();
	this->clear_flags();
}

Proxy	&Proxy::operator=(const Proxy &source) {
	_server.end_connexion();
	_server = source._server;
	_clients = source._clients;
	_cache = source._cache;
	_flags = source._flags;
	_poll_data = source._poll_data;
	_timeout = source._timeout;
	_to_server = source._to_server;
	_cl_base_pevents = source._cl_base_pevents;
	_cl_message_pevents = source._cl_message_pevents;
	_sv_pevents = source._sv_pevents;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Methods

// Clear all registered clients' sockets, but not the server socket.
// Clears : Clients, cache, pollfd (without server socket)
void		Proxy::end_all_connexions() {
	for (client_tree_type::iterator it = _clients.begin() ;
			it != _clients.end(); ) {
		client_tree_type::iterator tmp = it;
		it->second->end_connexion();
		delete it->second;
		++it;
		erase_cache(*tmp->second);
		erase_pollfd(*tmp->second);
		_clients.erase(tmp);
	}
}

void		Proxy::end_connexion(socket_type &target) {
	target.end_connexion();
	delete_client(target);
}

void			Proxy::switch_on() {
	this->init_server_socket();
}

void			Proxy::switch_off() {
	this->end_all_connexions();
	this->_poll_data.clear();
	this->_to_server.clear();
	this->_server.end_connexion();
}

void			Proxy::set_timeout(Proxy::milisecond_type timeout) { _timeout = timeout; }

void			Proxy::queuing() {
	fd_type		server_fd = _server.get_fd();

	set_flags();

	poll(_poll_data.data(), _poll_data.size(), _timeout);

	for (pollfd_type::iterator it = _poll_data.begin() ;
			it != _poll_data.end() ;
			++it) {
		if (it->revents == 0)
			continue;
		else if (it->fd == server_fd)
			_flags[it->revents]->handle_server(&_server);
		else {
			client_tree_type::iterator tmp = _clients.find(it->fd);
			if (tmp == _clients.end())
				std::clog << " -- [ERROR] Client not found. [" << it->fd << "] " << std::endl;
			else
				_flags[it->revents]->handle(tmp->second);
		}
	}
}

Proxy::api_type		Proxy::send_api() { return _to_server; }

// Take the list to handle() instiated classes then delete them.
// 'data' will be empty after function call.
void				Proxy::receive_api(api_type &data) {
	while (!data.empty()) {
		data.front()->handle();
		delete data.front();
		data.pop_front();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Private methodes

//////////////////////////////////////////////////////////////////////////
// Enders and kill : END

void		Proxy::clear_flags() {
	for (flag_tree_type::iterator it = _flags.begin() ; it != _flags.end() ; ++it )
		delete it->second;
	_flags.clear();
}

//////////////////////////////////////////////////////////////////////////
// Initialisation : INIT

void	Proxy::init_flags() {
	_flags.insert(std::make_pair(POLLIN, new Poll_in()));
	_flags.insert(std::make_pair(POLLPRI, new Poll_priority_in()));
	_flags.insert(std::make_pair(POLLNVAL, new Poll_invalid()));
	_flags.insert(std::make_pair(POLLHUP, new Poll_hang_up()));
	_flags.insert(std::make_pair(POLLERR, new Poll_error()));
}

// Create, bind, listen.
// Insert the socket in : cache, pollfd array.
// Set events in pollfd structure as a precaution.
// Only call in switch_on().
void	Proxy::init_server_socket() {
	_server.create_endpoint();
	_server.bind_socket();
	_server.listen_for_connexion(); // set on 5 maximum requested connexion (default)

	_cache.insert(std::make_pair(_server.get_fd(), cache_queue_type()));

	struct pollfd	server_sock;

	bzero(&server_sock, sizeof(pollfd));
	server_sock.fd = _server.get_fd();
	server_sock.events = POLLIN;
	_poll_data.push_back(server_sock);
}

void	Proxy::init_poll_events() {
	// Initialise events that will be requested for each clients.
	_cl_base_pevents = POLLIN | POLLPRI;
	_cl_message_pevents = POLLIN | POLLPRI | POLLOUT;
	_sv_pevents = POLLIN;
}

//////////////////////////////////////////////////////////////////////////
// Setters : SET

// All pollfd.events are updated, even the server's one.
void		Proxy::set_flags() {
	fd_type		server_fd = _server.get_fd();

	// Iterate on pollfd vector/array.
	for (Proxy::pollfd_type::iterator it = _poll_data.begin() ;
			it != _poll_data.end() ; ++it) {
		if (it->fd != server_fd) {
			Proxy::cache_tree_type::iterator it_cache = _cache.find(it->fd);
			// If there are pending messages for this client,
			// then use different flag set.
			if (!it_cache->second.empty())
				it->events = _cl_message_pevents;
			else
				it->events = _cl_base_pevents;
		}
		else
			it->events = _sv_pevents;
	}
}


//////////////////////////////////////////////////////////////////////////
// Modifiers : INSERT READ WRITE

void		Proxy::insert_client(const socket_type &new_client) {
	_clients.insert(std::make_pair(new_client.get_fd(), new socket_type(new_client)));
}

void		Proxy::insert_empty_cache(const socket_type &new_client) {
	_cache.insert(std::make_pair(new_client.get_fd(), cache_queue_type()));
}

void		Proxy::insert_pollfd(const socket_type &new_client) {
	struct pollfd	new_pollfd;

	bzero(&new_pollfd, sizeof(pollfd));
	new_pollfd.fd = new_client.get_fd();
	new_pollfd.events = _cl_base_pevents;
	_poll_data.push_back(new_pollfd);
}

// Insert the socket in : client tree, cache, pollfd array.
// Set events in pollfd structure as a precaution.
// Only call on Poll_in.handle_server().
void		Proxy::add_client(const socket_type &new_client) {
	insert_client(new_client);
	insert_empty_cache(new_client);
	insert_pollfd(new_client);
}

// Only call on Poll_in.handle().
int			Proxy::receive(const socket_type *client) {
	int		buffer_len = 513;
	char	buffer[buffer_len];

	bzero(&buffer, buffer_len);
	int		rtn = 0;
	while (true) {
	// MSG_DONTWAIT -> enable non-blocking operation.
		rtn = recv(client->get_fd(), &buffer, buffer_len - 1, MSG_DONTWAIT);
		if (rtn < 0) {
			if (errno != EAGAIN) {
				std::clog << "[ERROR] recv() failed. [" << client->get_fd()
					<< "] [" << client->get_address_readable() << "]" << std::endl;
				return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		}
		else if (rtn == 0) {
			std::clog << " --> Connexion closed. [" << client->get_fd() << "]"
				<< " [" << client->get_address_readable() << "]" << std::endl;
			// Connexion closed : launch POLLHUP after
			return -1;
		}
		_to_server.push_back(new Server_queue::Message(buffer, client));
		std::clog << " --> New message. [" << client->get_fd() << "]"
				<< " [" << client->get_address_readable() << "]" << std::endl;
		bzero(&buffer, buffer_len);
		rtn = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// Erasers : DELETE 

void		Proxy::erase_cache(const socket_type &target) {
	cache_queue_type	queue = _cache.find(target.get_fd())->second;

	while (!queue.empty()) {
		if (queue.front())
			free(queue.front());
		queue.pop_front();
	}
}

void		Proxy::erase_pollfd(const socket_type &target) {
	fd_type					fd_target = target.get_fd();
	pollfd_type::iterator 	it = _poll_data.begin();

	while (it != _poll_data.end()) {
		if (it->fd == fd_target) {
			_poll_data.erase(it);
			break;
		}
		++it;
	}
}

void		Proxy::erase_client_socket(const socket_type &target) {
	client_tree_type::iterator	it = _clients.find(target.get_fd());

	if (it != _clients.end()) {
		delete it->second;
		_clients.erase(target.get_fd());
	}
}

// Delete the socket in : client tree, cache, pollfd array.
void		Proxy::delete_client(const socket_type &client) {
	erase_pollfd(client);
	erase_cache(client);
	erase_client_socket(client);
}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Poll classes

Proxy::IPoll_handling::IPoll_handling()
	: _proxy(NULL) {
}

Proxy::IPoll_handling::IPoll_handling(Proxy &proxy)
	: _proxy(&proxy) { }

Proxy::IPoll_handling::~IPoll_handling() {
}

void	Proxy::IPoll_handling::handle(socket_type *socket) {
}
void	Proxy::IPoll_handling::handle_server(socket_type *server_socket) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_in : POLLIN

Proxy::Poll_in::Poll_in() { }

Proxy::Poll_in::Poll_in(Proxy &proxy) : IPoll_handling(proxy) { }

Proxy::Poll_in::~Poll_in() { }

void	Proxy::Poll_in::handle(socket_type *socket) {
	int		rtn = 0;
	rtn = _proxy->receive(socket);
	// FIXME : Error on recv()
	// if (rtn == EXIT_FAILURE)
		// return ;

	// If client disconnects
	if (rtn == -1)
		_proxy->_flags[POLLHUP]->handle(socket);
}

void	Proxy::Poll_in::handle_server(socket_type *server_socket) {
	_proxy->add_client(server_socket->accept_connexion());
	_proxy->_to_server.push_back(new Server_queue::Request_connexion(server_socket));
}


//////////////////////////////////////////////////////////////////////////
// Priority_in : POLLPRI

Proxy::Poll_priority_in::Poll_priority_in() {
}

Proxy::Poll_priority_in::~Poll_priority_in() {
}

Proxy::Poll_priority_in::Poll_priority_in(Proxy &proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_priority_in::handle(socket_type *socket) {
}

// Stay empty : server doesn't have POLLPRI flag.
void	Proxy::Poll_priority_in::handle_server(socket_type *socket) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_invalid : POLLNVAL

Proxy::Poll_invalid::Poll_invalid() {
}

Proxy::Poll_invalid::~Poll_invalid() {
}

Proxy::Poll_invalid::Poll_invalid(Proxy &proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_invalid::handle(socket_type *socket) {
}
void	Proxy::Poll_invalid::handle_server(socket_type *socket) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_hang_up : POLLHUP

Proxy::Poll_hang_up::Poll_hang_up() {
}

Proxy::Poll_hang_up::~Poll_hang_up() {
}

Proxy::Poll_hang_up::Poll_hang_up(Proxy &proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_hang_up::handle(socket_type *socket) {
	socket->end_connexion();
	_proxy->_to_server.push_back(new Server_queue::Client_disconnected(socket));
}

void	Proxy::Poll_hang_up::handle_server(socket_type *socket) {
	// FIXME : error to handle
}

//////////////////////////////////////////////////////////////////////////
// Poll_error : POLLERR

Proxy::Poll_error::Poll_error() {
}

Proxy::Poll_error::~Poll_error() {
}

Proxy::Poll_error::Poll_error(Proxy &proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_error::handle(socket_type *socket) {
}
void	Proxy::Poll_error::handle_server(socket_type *socket) {
}

