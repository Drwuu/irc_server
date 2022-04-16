/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guhernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 23:03:17 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/16 20:30:15 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Proxy.hpp"
#include <sys/wait.h>

// Should stay unaccessible
Proxy::Proxy()
	: _server(0, SOCK_STREAM), _clients(client_tree_type()), _cache(cache_tree_type()), _flags(flag_tree_type()), _poll_data(pollfd_type()),
	_timeout(60 * 1000), _to_server(api_type()),
	_cl_base_pevents(), _cl_message_pevents(), _sv_pevents() { }

Proxy::Proxy(const port_type &port)
	: _server(port, SOCK_STREAM), _clients(client_tree_type()), _cache(cache_tree_type()), _flags(flag_tree_type()), _poll_data(pollfd_type()),
	_timeout(60 * 1000), _to_server(api_type()),
	_cl_base_pevents(0), _cl_message_pevents(0), _sv_pevents(0),
	_cl_hang_up(0), _cl_invalid(0), _cl_error(0) {

		std::clog << " |--| Proxy initialisation ..." << std::endl;
		this->init_poll_events();
		this->init_flags();
		std::clog << " |--| Proxy initialisation done." << std::endl;
	}

Proxy::Proxy(const Proxy &source)
	: _server(source._server), _clients(source._clients), _cache(source._cache),
	_flags(source._flags), _poll_data(source._poll_data),
	_timeout(source._timeout),  _to_server(source._to_server),
	_cl_base_pevents(source._cl_base_pevents), _cl_message_pevents(source._cl_message_pevents),
	_sv_pevents(source._sv_pevents),
	_cl_hang_up(source._cl_hang_up), _cl_invalid(source._cl_invalid), _cl_error(source._cl_error) { }

Proxy::~Proxy() {
	std::clog << " |--| Proxy destruction ..." << std::endl;
	this->switch_off();
	this->clear_flags();
	std::clog << " |--| Proxy destruction done" << std::endl;
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
	_cl_hang_up = source._cl_hang_up;
	_cl_invalid = source._cl_invalid;
	_cl_error = source._cl_error;
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
		erase_cache(*tmp->second);
		erase_pollfd(*tmp->second);
		it->second->end_connexion();
		delete it->second;
		++it;
		_clients.erase(tmp);
	}
}

void		Proxy::end_connexion(socket_type &target) {
	int				sockfd = target.get_fd();
	std::string		address = target.get_address_readable();

	std::clog << " ---- End connexion on [" << sockfd << "] - [" << address << "] ... " << std::endl;
	delete_client(target);
	std::clog << " ---- Ended connexion on [" << sockfd << "] - [" << address << "]." << std::endl;
}

void			Proxy::switch_on() {
	std::clog << " |--| Proxy switch on ..." << std::endl;
	this->init_server_socket();
	std::clog << " |--| Proxy started." << std::endl;
}

void			Proxy::switch_off() {
	std::clog << " |--| Proxy switch off ..." << std::endl;
	this->end_all_connexions();
	this->_poll_data.clear();
	this->_to_server.clear();
	this->_server.end_connexion();
	std::clog << " |--| Proxy shut down " << std::endl;
}

void			Proxy::set_timeout(Proxy::milisecond_type timeout) { _timeout = timeout; }

void			Proxy::queuing() {
	std::clog << std::endl << std::endl;
	std::clog << " ---- Queuing ... " << std::endl;
	fd_type		server_fd = _server.get_fd();
	int			rtn = 0;

	set_flags();
	rtn = poll(_poll_data.data(), _poll_data.size(), _timeout);
	if (rtn == -1)
		std::clog << "POLL ERROR : " << strerror(errno) << std::endl;

	// Need to save the old pollfd size to secure the loop.
	// The _poll_data vector is modified if there is a new connexion
	// to the server.
	// As we loop on a vector that can be modified,
	// this is not safe to use iterators or _poll_data.size().
	int		size = _poll_data.size();

	for (int i = 0 ; i < size ; ++i) {
		// If there is no event trigered
		if (_poll_data[i].revents == 0)
			continue;
		// If there is a new connexion to the server
		else if (_poll_data[i].fd == server_fd) {
			_flags[_poll_data[i].revents]->handle_server(&_server);
		}
		// Else, a pending connexion trigered an event
		else {
			client_tree_type::iterator tmp = _clients.find(_poll_data[i].fd);
			if (tmp == _clients.end())
				std::clog << " ---- [ERROR] Client not found. [" << _poll_data[i].fd << "] " << std::endl;
			// FIXME -> the tree should always find a value. But it doesn't.
			else { 
				flag_tree_type::iterator	it_event = _flags.find(_poll_data[i].revents);
				if (it_event == _flags.end()) { 
					std::clog << " ---- [ERROR] Flag not found. [" << _poll_data[i].fd << "] ";
					std::clog << " -- flag : " << _poll_data[i].revents << std::endl;
					std::clog << " -- errno : " << " [" << errno << "] " << strerror(errno) << std::endl;
					break;
				}
				it_event->second->handle(tmp->second);
			}
		}
	}
	std::clog << " ---- Queuing done. " << std::endl;
}

// Should always preced 'receive_api'.
Proxy::api_type		Proxy::send_api() {
	std::clog << " <--- API Sending ... " << std::endl;
	return _to_server;
	std::clog << " <--- API Sent. " << std::endl;
}

// Take the list to handle() instiated classes then delete them.
// 'data' will be empty after function call.
// Should always be preceded by 'send_api'
void				Proxy::receive_api(api_type &data) {
	std::clog << " ---> API Reception. " << std::endl;
	_to_server.clear();

	std::clog << " ---> API Handling ... " << std::endl;
	while (!data.empty()) {
		data.front()->handle();
		delete data.front();
		data.pop_front();
	}
	std::clog << " ---> API Handled. " << std::endl;
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
	std::clog << " |--| FLAG initialisation ... " << std::endl;

	// _flags.insert(std::make_pair(POLLPRI, new Poll_priority_in(this)));
	_flags.insert(std::make_pair(POLLIN, new Poll_in(this)));

	_flags.insert(std::make_pair(POLLNVAL, new Poll_invalid(this)));
	_flags.insert(std::make_pair(_cl_invalid, new Poll_invalid(this)));

	_flags.insert(std::make_pair(POLLHUP, new Poll_hang_up(this)));
	_flags.insert(std::make_pair(_cl_hang_up, new Poll_hang_up(this)));

	_flags.insert(std::make_pair(POLLERR, new Poll_error(this)));
	_flags.insert(std::make_pair(_cl_error, new Poll_error(this)));

	_flags.insert(std::make_pair(POLLOUT, new Poll_out(this)));
	_flags.insert(std::make_pair(POLLWRNORM, new Poll_out(this)));
	std::clog << " |--| FLAG inialised. " << std::endl;
}

// Create, bind, listen.
// Insert the socket in : cache, pollfd array.
// Set events in pollfd structure as a precaution.
// Only call in switch_on().
void	Proxy::init_server_socket() {
	std::clog << " |--| Server Socket initialisation ... " << std::endl;
	_server.create_endpoint();
	_server.bind_socket();
	_server.listen_for_connexion(); // set on 5 maximum requested connexion (default)

	_cache.insert(std::make_pair(_server.get_fd(), cache_queue_type()));

	struct pollfd	server_sock;

	bzero(&server_sock, sizeof(pollfd));
	server_sock.fd = _server.get_fd();
	server_sock.events = POLLIN;
	_poll_data.push_back(server_sock);
	std::clog << " |--| Server Socket initialised." << std::endl;
}

void	Proxy::init_poll_events() {
	// Initialise events that will be requested for each clients.
	_cl_base_pevents = POLLIN;
	_cl_message_pevents = POLLIN | POLLOUT;
	_sv_pevents = POLLIN;

	_cl_hang_up =  POLLHUP | POLLIN;
	_cl_invalid =  POLLNVAL | POLLIN;
	_cl_error = POLLERR | POLLIN;
	// 1 (POLLIN)
	// 4 (POLLIN | POLLPRI)
	// 5 (POLLIN | POLLOUT)
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
		it->revents = 0;
	}
	std::clog << " ---- Flag updated." << std::endl;
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
	// MSG_DONTWAIT -> enable non-blocking operation.
	rtn = recv(client->get_fd(), &buffer, buffer_len - 1, MSG_DONTWAIT);
	if (rtn < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		std::clog << "[ERROR] recv() failed. [" << client->get_fd()
			<< "] [" << client->get_address_readable() << "] :"
			<< strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	else if (rtn == 0) {
		std::clog << " ---> Connexion closed. [" << client->get_fd() << "]"
			<< " [" << client->get_address_readable() << "]" << std::endl;
		// Connexion closed : launch POLLHUP after
		return -1;
	}
	_to_server.push_back(new Server_queue::Message(buffer, client));
	std::clog << " ---> New message. [" << client->get_fd() << "]"
		<< " [" << client->get_address_readable() << "]" << std::endl;
	bzero(&buffer, buffer_len);
	return EXIT_SUCCESS;
}

int			Proxy::send_to_client(const socket_type *client, const data_type data) {
	int		rtn = 0;
	size_t	buffer_len = 513;
	char	buffer[buffer_len];

	if (buffer_len >= strlcpy(buffer, data, buffer_len - 1))
		std::clog << " ---- [ERROR] Data sent had been truncated. "
			<< "[" << client->get_fd() << "] "
			<< "[" << client->get_address_readable() << "]" << std::endl;

	bzero(&buffer, buffer_len);
	rtn = send(client->get_fd(), buffer, buffer_len, 0);
	if (rtn < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		std::clog << " ---- [ERROR] send() failed. [" << client->get_fd()
			<< "] [" << client->get_address_readable() << "] : "
			<< strerror(errno) << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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
		int		sockfd = it->second->get_fd();
		it->second->end_connexion();
		delete it->second;
		_clients.erase(sockfd);
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

Proxy::IPoll_handling::IPoll_handling(Proxy *proxy)
	: _proxy(proxy) { }

Proxy::IPoll_handling::~IPoll_handling() {
}

void	Proxy::IPoll_handling::handle(socket_type *) {
}
void	Proxy::IPoll_handling::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_in : POLLIN

Proxy::Poll_in::Poll_in() { }

Proxy::Poll_in::Poll_in(Proxy *proxy) : IPoll_handling(proxy) { }

Proxy::Poll_in::~Poll_in() { }

void	Proxy::Poll_in::handle(socket_type *socket) {
	std::clog << " ---> New message incomming ..." << std::endl;
	int		rtn = 0;
	rtn = _proxy->receive(socket);
	// FIXME : Error on recv()
	if (rtn == EXIT_FAILURE)
		return ;
	// If client disconnects
	else if (rtn == -1)  {
		_proxy->_flags[POLLHUP]->handle(socket);
	}
}

void	Proxy::Poll_in::handle_server(socket_type *server_socket) {
	std::clog << " ---> New requested connexion ..." << std::endl;
	_proxy->add_client(server_socket->accept_connexion());
	_proxy->_to_server.push_back(new Server_queue::Request_connexion(server_socket));
	std::clog << " ---> New requested connexion done." << std::endl;
}


//////////////////////////////////////////////////////////////////////////
// Priority_in : POLLPRI

Proxy::Poll_priority_in::Poll_priority_in() {
}

Proxy::Poll_priority_in::~Poll_priority_in() {
}

Proxy::Poll_priority_in::Poll_priority_in(Proxy *proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_priority_in::handle(socket_type *) {
}

// Stay empty : server doesn't have POLLPRI flag.
void	Proxy::Poll_priority_in::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_invalid : POLLNVAL

Proxy::Poll_invalid::Poll_invalid() {
}

Proxy::Poll_invalid::~Poll_invalid() {
}

Proxy::Poll_invalid::Poll_invalid(Proxy *proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_invalid::handle(socket_type *) {
}
void	Proxy::Poll_invalid::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_hang_up : POLLHUP

Proxy::Poll_hang_up::Poll_hang_up() {
}

Proxy::Poll_hang_up::~Poll_hang_up() {
}

Proxy::Poll_hang_up::Poll_hang_up(Proxy *proxy) : IPoll_handling(proxy)
{ }

// Doesn't erase the client or the Server cannot identify
// the disconnected socket.
// Deletion will proced after deletion confirmation from
// the server.
void	Proxy::Poll_hang_up::handle(socket_type *socket) {
	std::clog << " ---> Client disconnected." << std::endl;
	std::clog << " ---> Client disconnection handling ..." << std::endl;
	_proxy->_to_server.push_back(new Server_queue::Client_disconnected(socket));
	std::clog << " ---> Client disconnection done." << std::endl;
}

void	Proxy::Poll_hang_up::handle_server(socket_type *socket) {
	std::clog << " |--| ERROR POLLHUP " << std::endl;
	std::clog << "socket : " << socket->get_fd() << std::endl;
}

//////////////////////////////////////////////////////////////////////////
// Poll_error : POLLERR

Proxy::Poll_error::Poll_error() {
}

Proxy::Poll_error::~Poll_error() {
}

Proxy::Poll_error::Poll_error(Proxy *proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_error::handle(socket_type *) {
}
void	Proxy::Poll_error::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_out : POLLERR

Proxy::Poll_out::Poll_out() {
}

Proxy::Poll_out::~Poll_out() {
}

Proxy::Poll_out::Poll_out(Proxy *proxy) : IPoll_handling(proxy)
{ }

void	Proxy::Poll_out::handle(socket_type *client) {
	cache_tree_type::iterator		it_cache = _proxy->_cache.find(client->get_fd());
	if (it_cache == _proxy->_cache.end()) {
		std::clog << " <--- [ERROR] : Wrong flag, no data to send to client "
			<< "[" << client->get_fd() << "] [" << client->get_address_readable() << "]"
			<< std::endl;
		return;
	}
	else if (it_cache->second.empty()) {
		std::clog << " <--- [ERROR] : Wrong flag, no pending messages to send "
			<< "[" << client->get_fd() << "] [" << client->get_address_readable() << "]"
			<< std::endl;
		return;
	}
	// FIXME : error on send ?
	_proxy->send_to_client(client, it_cache->second.front());
	it_cache->second.pop_front();
}

void	Proxy::Poll_out::handle_server(socket_type *) {
}
