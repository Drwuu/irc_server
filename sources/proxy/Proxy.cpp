#include "../../headers/proxy/Proxy.hpp"
#include <cstring>
#include <sys/wait.h>

// Should stay unaccessible
irc::Proxy::Proxy()
	: _server(0, SOCK_STREAM), _clients(client_tree_type()), _cache(cache_tree_type()), _flags(flag_tree_type()), _poll_data(pollfd_type()),
	_timeout(60 * 1000), _to_server(api_type()),
	_cl_base_pevents(), _cl_message_pevents(), _sv_pevents() { }

irc::Proxy::Proxy(const port_type &port)
	: _server(port, SOCK_STREAM), _clients(client_tree_type()), _cache(cache_tree_type()), _flags(flag_tree_type()), _poll_data(pollfd_type()),
	_timeout(60 * 1000), _to_server(api_type()),
	_cl_base_pevents(0), _cl_message_pevents(0), _sv_pevents(0),
	_cl_hang_up(0), _cl_invalid(0), _cl_error(0) {

		std::clog << " |--| irc::Proxy initialisation ..." << std::endl;
		this->init_poll_events();
		this->init_flags();
		std::clog << " |--| irc::Proxy initialisation done." << std::endl;
	}

irc::Proxy::Proxy(const irc::Proxy &source)
	: _server(source._server), _clients(source._clients), _cache(source._cache),
	_flags(source._flags), _poll_data(source._poll_data),
	_timeout(source._timeout),  _to_server(source._to_server),
	_cl_base_pevents(source._cl_base_pevents), _cl_message_pevents(source._cl_message_pevents),
	_sv_pevents(source._sv_pevents),
	_cl_hang_up(source._cl_hang_up), _cl_invalid(source._cl_invalid), _cl_error(source._cl_error) { }

irc::Proxy::~Proxy() {
	std::clog << " |--| irc::Proxy destruction ..." << std::endl;
	this->switch_off();
	this->clear_flags();
	std::clog << " |--| irc::Proxy destruction done" << std::endl;
}

irc::Proxy	&irc::Proxy::operator=(const irc::Proxy &source) {
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

// Clear all registered clients' sockets, but not the Server socket.
// Clears : Clients, cache, pollfd (without Server socket)
void		irc::Proxy::end_all_connexions() {
	for (client_tree_type::iterator it = _clients.begin() ; it != _clients.end(); ) {
		client_tree_type::iterator tmp = it;
		erase_cache(*tmp->second);
		erase_pollfd(*tmp->second);
		it->second->end_connexion();
		delete it->second;
		++it;
		_clients.erase(tmp);
	}
}

void		irc::Proxy::end_connexion(socket_type &target) {
	int				sockfd = target.get_fd();
	std::string		address = target.get_address_readable();

	std::clog << " ---- End connexion on [" << sockfd << "] - [" << address << "] ... " << std::endl;
	delete_client(target);
	std::clog << " ---- Ended connexion on [" << sockfd << "] - [" << address << "]." << std::endl;
}

void			irc::Proxy::switch_on() {
	std::clog << " |--| irc::Proxy switch on ..." << std::endl;
	this->init_server_socket();
	std::clog << " |--| irc::Proxy started." << std::endl;
}

void			irc::Proxy::switch_off() {
	std::clog << " |--| irc::Proxy switch off ..." << std::endl;
	this->end_all_connexions();
	this->_poll_data.clear();
	this->_to_server.clear();
	this->_server.end_connexion();
	std::clog << " |--| irc::Proxy shut down " << std::endl;
}

void			irc::Proxy::set_timeout(irc::Proxy::milisecond_type timeout) { _timeout = timeout; }

void			irc::Proxy::queuing() {
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
	// to the Server.
	// As we loop on a vector that can be modified,
	// this is not safe to use iterators or _poll_data.size().
	int		size = _poll_data.size();

	for (int i = 0 ; i < size ; ++i) {
		// If there is no event trigered
		if (_poll_data[i].revents == 0)
			continue;
		// If there is a new connexion to the Server
		else if (_poll_data[i].fd == server_fd) {
			_flags[_poll_data[i].revents]->handle_server(&_server);
		}
		// Else, a pending connexion trigered an event
		// Throw if : error, disconnection, unknown parameters (client)
		else {
			try {
				client_tree_type::iterator tmp = _clients.find(_poll_data[i].fd);
				if (tmp == _clients.end()) {
					std::stringstream	ss;
					ss << " ---- [ERROR] Client not found. [" << _poll_data[i].fd  << "] " << std::endl;
					throw Unknown_client_exception(ss.str());
				}
				// FIXME -> the tree should always find a value. But it doesn't.
				else {
					flag_tree_type::iterator	it_event = _flags.find(_poll_data[i].revents);
					if (it_event == _flags.end()) {
						std::stringstream	ss;
						ss << " ---- [ERROR] Flag not found. [" << _poll_data[i].fd << "] ";
						ss << " -- flag : " << _poll_data[i].revents << std::endl;
						ss << " -- errno : " << " [" << errno << "] " << strerror(errno) << std::endl;
						throw Error_exception(ss.str());
					}
					it_event->second->handle(tmp->second);
				}
			}
			catch (const irc::Proxy::Disconnection_exception &e) {
				this->_flags[POLLHUP]->handle(e.get_socket());
			}
			catch (const irc::Proxy::Unknown_client_exception &e) {
				std::clog << e.what();
			}
			catch (const irc::Proxy::Error_exception &e) {
				std::clog << e.what();
			}
		}
	}
	std::clog << " ---- Queuing done. " << std::endl;
}

// Should always preced 'receive_api'.
irc::Proxy::api_type		irc::Proxy::send_api() {
	std::clog << " <--- API Sending ... " << std::endl;
	return _to_server;
	std::clog << " <--- API Sent. " << std::endl;
}

// Take the list to handle() instiated classes then delete them.
// 'data' will be empty after function call.
// Should always be preceded by 'send_api'
void				irc::Proxy::receive_api(api_type &data) {
	std::clog << " ---> API Reception. " << std::endl;
	_to_server.clear();

	std::clog << " ---> API Handling ... " << std::endl;
	for (; !data.empty() ; data.pop_front()) {
		try {
			data.front()->handle(*this);
		}
		catch (const irc::Proxy::Disconnection_exception &e) {
			this->_flags[POLLHUP]->handle(e.get_socket());
		}
		catch (const irc::Proxy::Unknown_client_exception &e) {
			std::clog << e.what();
		}
		catch (const irc::Proxy::Error_exception &e) {
			std::clog << e.what();
		}
		delete data.front();
	}
	std::clog << " ---> API Handled. " << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Private methodes

//////////////////////////////////////////////////////////////////////////
// Enders and kill : END

void		irc::Proxy::clear_flags() {
	for (flag_tree_type::iterator it = _flags.begin() ; it != _flags.end() ; ++it )
		delete it->second;
	_flags.clear();
}

//////////////////////////////////////////////////////////////////////////
// Initialisation : INIT

void	irc::Proxy::init_flags() {
	std::clog << " |--| FLAG initialisation ... " << std::endl;

	// _flags.insert(std::make_pair(POLLPRI, new Poll_priority_in(this)));
	_flags.insert(std::make_pair(POLLIN, new Poll_in(this)));

	_flags.insert(std::make_pair(POLLNVAL, new Poll_invalid(this)));
	_flags.insert(std::make_pair(_cl_invalid, new Poll_invalid(this)));

	_flags.insert(std::make_pair(POLLHUP, new Poll_hang_up(this)));
	_flags.insert(std::make_pair(_cl_hang_up, new Poll_hup_in(this)));

	_flags.insert(std::make_pair(POLLERR, new Poll_error(this)));
	_flags.insert(std::make_pair(_cl_error, new Poll_error(this)));

	_flags.insert(std::make_pair(POLLOUT, new Poll_out(this)));
	_flags.insert(std::make_pair(_cl_message_pevents, new Poll_out(this)));
	_flags.insert(std::make_pair(POLLWRNORM, new Poll_out(this)));
	std::clog << " |--| FLAG inialised. " << std::endl;
}

// Create, bind, listen.
// Insert the socket in : cache, pollfd array.
// Set events in pollfd structure as a precaution.
// Only call in switch_on().
void	irc::Proxy::init_server_socket() {
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

void	irc::Proxy::init_poll_events() {
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

// All pollfd.events are updated, even the Server's one.
void		irc::Proxy::set_flags() {
	fd_type		server_fd = _server.get_fd();

	// Iterate on pollfd vector/array.
	for (irc::Proxy::pollfd_type::iterator it = _poll_data.begin() ;
			it != _poll_data.end() ; ++it) {
		if (it->fd != server_fd) {
			irc::Proxy::cache_tree_type::iterator it_cache = _cache.find(it->fd);
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

void		irc::Proxy::insert_client(const socket_type &new_client) {
	_clients.insert(std::make_pair(new_client.get_fd(), new socket_type(new_client)));
}

void		irc::Proxy::insert_empty_cache(const socket_type &new_client) {
	_cache.insert(std::make_pair(new_client.get_fd(), cache_queue_type()));
}

void		irc::Proxy::insert_pollfd(const socket_type &new_client) {
	struct pollfd	new_pollfd;

	bzero(&new_pollfd, sizeof(pollfd));
	new_pollfd.fd = new_client.get_fd();
	new_pollfd.events = _cl_base_pevents;
	_poll_data.push_back(new_pollfd);
}

// Insert the socket in : client tree, cache, pollfd array.
// Set events in pollfd structure as a precaution.
// Only call on Poll_in.handle_server().
void		irc::Proxy::add_client(const socket_type &new_client) {
	insert_client(new_client);
	insert_empty_cache(new_client);
	insert_pollfd(new_client);
}

// Only call on Poll_in.handle().
void		irc::Proxy::receive(socket_type *client) {
	int		buffer_len = 513;
	char	buffer[buffer_len];

	bzero(&buffer, buffer_len);
	int		rtn = 0;
	// MSG_DONTWAIT -> enable non-blocking operation.
	rtn = recv(client->get_fd(), &buffer, buffer_len - 1, MSG_DONTWAIT);
	if (rtn < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		std::stringstream	ss;
		ss << "[ERROR] recv() failed. [" << client->get_fd()
			<< "] [" << client->get_address_readable() << "] :"
			<< strerror(errno) << std::endl;
		throw Error_exception(ss.str());
	}
	// Connexion closed : launch POLLHUP after
	else if (rtn == 0) {
		std::stringstream	ss;
		ss << " ---> Connexion closed. [" << client->get_fd() << "]"
			<< " [" << client->get_address_readable() << "]" << std::endl;
		throw Disconnection_exception(ss.str(), client);
	}
	_to_server.push_back(new irc::Server_queue::Message(buffer, client));
	std::clog << " ---> New message. [" << client->get_fd() << "]"
		<< " [" << client->get_address_readable() << "]" << std::endl;
	bzero(&buffer, buffer_len);
}

void		irc::Proxy::push_back_queue(fd_type client_fd, data_type data) {
	cache_tree_type::iterator	it = _cache.find(client_fd);

	if (it == _cache.end()) {
		std::stringstream	ss;
		ss << " ---- [ERROR] Cache : client not found. ["
			<< client_fd << "] data :[" << data << "] " << std::endl;
		throw Error_exception(ss.str());
	}
	it->second.push_back(data);
}

void		irc::Proxy::send_to_client(const socket_type *client, const data_type data) {
	int		rtn = 0;
	size_t	buffer_len = 513;
	char	buffer[buffer_len];

	bzero(buffer, buffer_len);
	if (strlen(data) > strlcpy(buffer, data, buffer_len - 1) )
		std::clog << "[DETAIL] : Data sent had been truncated" << std::endl;

	std::clog << " \n ======================================================== DATA SENT : " << buffer << " ==" << std::endl;

	rtn = send(client->get_fd(), buffer, strlen(buffer), 0);
	if (rtn < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		std::stringstream	ss;
		ss << " ---- [ERROR] send() failed. [" << client->get_fd()
			<< "] [" << client->get_address_readable() << "] : "
			<< strerror(errno) << std::endl;
		throw Error_exception(ss.str());
	}
}

//////////////////////////////////////////////////////////////////////////
// Erasers : DELETE

void		irc::Proxy::erase_cache(const socket_type &target) {
	cache_queue_type	queue = _cache.find(target.get_fd())->second;

	while (!queue.empty()) {
		delete queue.front();
		queue.pop_front();
	}
	_cache.erase(target.get_fd());
}

void		irc::Proxy::erase_pollfd(const socket_type &target) {
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

void		irc::Proxy::erase_client_socket(const socket_type &target) {
	client_tree_type::iterator	it = _clients.find(target.get_fd());

	if (it != _clients.end()) {
		int		sockfd = it->second->get_fd();
		it->second->end_connexion();
		delete it->second;
		_clients.erase(sockfd);
	}
}

// Delete the socket in : client tree, cache, pollfd array.
void		irc::Proxy::delete_client(const socket_type &client) {
	erase_pollfd(client);
	erase_cache(client);
	erase_client_socket(client);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Poll classes

irc::Proxy::IPoll_handling::IPoll_handling()
	: _proxy(NULL) {
}

irc::Proxy::IPoll_handling::IPoll_handling(irc::Proxy *proxy)
	: _proxy(proxy) { }

irc::Proxy::IPoll_handling::~IPoll_handling() {
}

void	irc::Proxy::IPoll_handling::handle(socket_type *) {
}
void	irc::Proxy::IPoll_handling::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_in : POLLIN

irc::Proxy::Poll_in::Poll_in() { }

irc::Proxy::Poll_in::Poll_in(irc::Proxy *proxy) : IPoll_handling(proxy) { }

irc::Proxy::Poll_in::~Poll_in() { }

void	irc::Proxy::Poll_in::handle(socket_type *socket) {
	std::clog << " ---> New message incomming ..." << std::endl;
	_proxy->receive(socket);
}

void	irc::Proxy::Poll_in::handle_server(socket_type *server_socket) {
	std::clog << " ---> New requested connexion ..." << std::endl;
	_proxy->add_client(server_socket->accept_connexion());
	_proxy->_to_server.push_back(new irc::Server_queue::Request_connexion(server_socket));
	std::clog << " ---> New requested connexion done." << std::endl;
}


//////////////////////////////////////////////////////////////////////////
// Priority_in : POLLPRI

irc::Proxy::Poll_priority_in::Poll_priority_in() {
}

irc::Proxy::Poll_priority_in::~Poll_priority_in() {
}

irc::Proxy::Poll_priority_in::Poll_priority_in(irc::Proxy *proxy) : IPoll_handling(proxy)
{ }

void	irc::Proxy::Poll_priority_in::handle(socket_type *) {
}

// Stay empty : Server doesn't have POLLPRI flag.
void	irc::Proxy::Poll_priority_in::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_invalid : POLLNVAL

irc::Proxy::Poll_invalid::Poll_invalid() {
}

irc::Proxy::Poll_invalid::~Poll_invalid() {
}

irc::Proxy::Poll_invalid::Poll_invalid(irc::Proxy *proxy) : IPoll_handling(proxy)
{ }

void	irc::Proxy::Poll_invalid::handle(socket_type *) {
}
void	irc::Proxy::Poll_invalid::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_hang_up : POLLHUP

irc::Proxy::Poll_hang_up::Poll_hang_up() {
}

irc::Proxy::Poll_hang_up::~Poll_hang_up() {
}

irc::Proxy::Poll_hang_up::Poll_hang_up(irc::Proxy *proxy) : IPoll_handling(proxy)
{ }

// Doesn't erase the client or the Server cannot identify
// the disconnected socket.
// Deletion will proced after deletion confirmation from
// the Server.
void	irc::Proxy::Poll_hang_up::handle(socket_type *socket) {
	std::clog << " ---> Client disconnected." << std::endl;
	std::clog << " ---> Client disconnection handling ..." << std::endl;
	_proxy->_to_server.push_back(new irc::Server_queue::Client_disconnected(socket));
	std::clog << " ---> Client disconnection done." << std::endl;
}

void	irc::Proxy::Poll_hang_up::handle_server(socket_type *socket) {
	std::clog << " |--| ERROR POLLHUP " << std::endl;
	std::clog << "socket : " << socket->get_fd() << std::endl;
}

//////////////////////////////////////////////////////////////////////////
// Poll_error : POLLERR

irc::Proxy::Poll_error::Poll_error() {
}

irc::Proxy::Poll_error::~Poll_error() {
}

irc::Proxy::Poll_error::Poll_error(irc::Proxy *proxy) : IPoll_handling(proxy)
{ }

void	irc::Proxy::Poll_error::handle(socket_type *) {
}
void	irc::Proxy::Poll_error::handle_server(socket_type *) {
}

//////////////////////////////////////////////////////////////////////////
// Poll_out : POLLOUT

irc::Proxy::Poll_out::Poll_out() {
}

irc::Proxy::Poll_out::~Poll_out() {
}

irc::Proxy::Poll_out::Poll_out(irc::Proxy *proxy) : IPoll_handling(proxy)
{ }

void	irc::Proxy::Poll_out::handle(socket_type *client) {
	cache_tree_type::iterator		it_cache = _proxy->_cache.find(client->get_fd());
	if (it_cache == _proxy->_cache.end()) {
		std::stringstream	ss;
		ss << " <--- [ERROR] : Wrong flag, no data to send to client "
			<< "[" << client->get_fd() << "] [" << client->get_address_readable() << "]"
			<< std::endl;
		throw Error_exception(ss.str());
	}
	else if (it_cache->second.empty()) {
		std::stringstream	ss;
		ss << " <--- [ERROR] : Wrong flag, no pending messages to send "
			<< "[" << client->get_fd() << "] [" << client->get_address_readable() << "]"
			<< std::endl;
		throw Error_exception(ss.str());
	}
	// FIXME : error on send ?
	_proxy->send_to_client(client, it_cache->second.front());
	const char *tmp = it_cache->second.front();
	if (tmp != NULL)
		free((void *)tmp);
	it_cache->second.front() = NULL;
	it_cache->second.pop_front();
}

void	irc::Proxy::Poll_out::handle_server(socket_type *) { }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Exceptions : Disconnection, Unkown client, Error

irc::Proxy::Disconnection_exception::Disconnection_exception(std::string content, socket_type *client) throw() : _content(content), _client(client) { }
irc::Proxy::Disconnection_exception::Disconnection_exception(const Disconnection_exception& other) throw() : _content(other._content) { }
irc::Proxy::Disconnection_exception::~Disconnection_exception() throw() { }
irc::Proxy::Disconnection_exception		&irc::Proxy::Disconnection_exception::operator=(const Disconnection_exception& ) { return *this; }
const char	*irc::Proxy::Disconnection_exception::what() const throw() { return _content.c_str(); }

irc::Proxy::socket_type		*irc::Proxy::Disconnection_exception::get_socket() const { return _client; }

//////////////////////////////////////////////////////////////////////////
//
irc::Proxy::Unknown_client_exception::Unknown_client_exception(std::string content) throw() : _content(content) { }
irc::Proxy::Unknown_client_exception::Unknown_client_exception(const Unknown_client_exception& other) throw() : _content(other._content) { }
irc::Proxy::Unknown_client_exception::~Unknown_client_exception() throw() { }
irc::Proxy::Unknown_client_exception		&irc::Proxy::Unknown_client_exception::operator=(const Unknown_client_exception& ) { return *this; }
const char	*irc::Proxy::Unknown_client_exception::what() const throw() { return _content.c_str(); }


//////////////////////////////////////////////////////////////////////////
//
irc::Proxy::Error_exception::Error_exception(std::string content) throw() : _content(content) { }
irc::Proxy::Error_exception::Error_exception(const Error_exception& other) throw() : _content(other._content) { }
irc::Proxy::Error_exception::~Error_exception() throw() { }
irc::Proxy::Error_exception		&irc::Proxy::Error_exception::operator=(const Error_exception& ) { return *this; }
const char	*irc::Proxy::Error_exception::what() const throw() { return _content.c_str(); }


//////////////////////////////////////////////////////////////////////////
// Poll_hup_in : POLLHUP | POLLIN

irc::Proxy::Poll_hup_in::Poll_hup_in() { }
irc::Proxy::Poll_hup_in::~Poll_hup_in() { }

irc::Proxy::Poll_hup_in::Poll_hup_in(Proxy *proxy) : IPoll_handling(proxy) { }

void	irc::Proxy::Poll_hup_in::handle(socket_type *socket) {
	_proxy->_flags[POLLIN]->handle(socket);
	_proxy->_flags[POLLHUP]->handle(socket);
}

void	irc::Proxy::Poll_hup_in::handle_server(socket_type *) { }

//////////////////////////////////////////////////////////////////////////
// Poll_out_in : POLLOUT | POLLIN

irc::Proxy::Poll_out_in::Poll_out_in() { }
irc::Proxy::Poll_out_in::~Poll_out_in() { }

irc::Proxy::Poll_out_in::Poll_out_in(Proxy *proxy) : IPoll_handling(proxy) { }

void	irc::Proxy::Poll_out_in::handle(socket_type *socket) {
	_proxy->_flags[POLLIN]->handle(socket);
	_proxy->_flags[POLLOUT]->handle(socket);
}

void	irc::Proxy::Poll_out_in::handle_server(socket_type *) { }
