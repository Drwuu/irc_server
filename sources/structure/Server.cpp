#include "../../headers/structure/Server.hpp"
#include <cstddef>

namespace irc {
/* Constructors & Destructors */
	Server::Server(): _line("INVITE toto prout"), _map(), _parser(parser()) {
		_map.insert(std::make_pair("INVITE", new invite()));
		// _map.insert(std::make_pair("KICK", new irc::kick()));
		// _map.insert(std::make_pair("MODE", new irc::mode()));
		User user;
		user.set_nickname("toto");
		add_user(&user);
		Channel chan("prout");
		add_channel(chan);
		// chan.add_user(&user);
		// dprintf (2, "Server() | chan =  %s\n", chan.get_name().c_str());
		command *cmd = parse_line(user);
		(void)cmd;
		// admin.join_channel(*this, "PROUT"); //FIXME : doubled user in chan
		// vec_user vec =  get_user_list();
		// for (vec_cit_user it = vec.begin(); it != vec.end(); it++)
		// 	dprintf (2, "it =  %s\n", (*it)->get_nickname().c_str());
		// vec_user chans =  chan.get_user_list();
		// for (vec_cit_user it = chans.begin(); it != chans.end(); it++)
		// 	dprintf (2, "chans =  %s\n", (*it)->get_nickname().c_str());
	};
	Server::Server(std::string password,std::string port): _password(password), _port(port) {
	}
	Server::Server(Server const &src) {
		*this = src;
	};
	Server::~Server() {
		for (irc::map_iterator_cmd it = _map.begin(); it != _map.end(); it++)
			if (it->second)
				delete it->second;
		_map.clear();
	};

/* Operators */
	Server& Server::operator=(Server const &src) {
		_name = src._name;
		_label = src._label;
		_hostname = src._hostname;
		_password = src._password;
		_port = src._port;
		_ip = src._ip;
		_version = src._version;
		_motd = src._motd;
		_date = src._date;
		_oper_id = src._oper_id;
		_oper_password = src._oper_password;
		_time = src._time;
		_user_list = src._user_list;
		_channel_list = src._channel_list;
		_ban_list = src._ban_list;
		_line = src._line;
		_map = src._map;
		return *this;
	};

/* Getters */
	irc::map_cmd const Server::get_map() const {
		return this->_map;
	};
	std::vector<Channel *> const Server::get_channel_list() const {
		return (this->_channel_list);
	}
	std::vector<User *> Server::get_user_list() const {
		return (this->_user_list);
	}
	std::string const Server::get_name() const {
		return (this->_name);
	}
	std::string const Server::get_hostname() const {
		return (this->_hostname);
	}
	std::string const Server::get_password() const {
		return (this->_password);
	}
	std::string const Server::get_port() const {
		return (this->_port);
	}
	std::string const Server::get_ip() const {
		return (this->_ip);
	}

/* Setters */
	void Server::set_name(std::string name){
		this->_name = name;
	}
	void Server::set_label(std::string label){
		this->_label = label;
	}
	void Server::set_hostname(std::string hostname){
		this->_hostname = hostname;
	}
	void Server::set_password(std::string password){
		this->_password = password;
	}
	void Server::set_port(std::string port){
		this->_port = port;
	}
	void Server::set_ip(std::string ip){
		this->_ip = ip;
	}
	void Server::set_motd(std::string motd){
		this->_motd = motd;
	}

/* Functions */
	vec_cit_user const Server::find_nickname(string const &nick, vec_user const &users) const {
		vec_cit_user it = users.begin();
		for (; it != users.end(); it++) {
			string nickname = (*it)->get_nickname();
			if (nickname == nick)
				return it;
		}
		return users.end();
	};
	vec_cit_user const Server::find_username(string const &name, vec_user const &users) const {
		vec_cit_user it = users.begin();
		for (; it != users.end(); it++) {
			string username = (*it)->get_username();
			if (username == name)
				return it;
		}
		return users.end();
	};
	vec_cit_chan const Server::find_chan_name(string const &chan, vec_chan const &channel) const {
		vec_cit_chan it = channel.begin();
		for (; it != channel.end(); it++) {
			string chanName = (*it)->get_name();
			if (chanName == chan)
				return it;
		}
		return channel.end();
	};
	command *Server::parse_line(User const &user) const {
		command *cmd = NULL;
		try {
			cmd = _parser.get_command(_line, _map)->second;
			cmd->set_args(_parser.get_args(_line));
			cmd->is_valid_args(this, user);
			return cmd;
		}
		catch (irc::error &e) {
			std::cout << e.what() << std::endl;
		}
		return cmd;
	};


	User * Server::get_user_from_socket(Socket<Address_ipv6> const *sock) {
		vec_cit_user it = _user_list.begin();
		for (; it != _user_list.end(); it++) {
			if ((*it)->get_socket() == sock)
				return (*it);
		}
		User * user = new User(sock);
		this->add_user(user);
		return user;
	};

	void Server::add_user(User * user) {
		this->_user_list.push_back(user);
	}
	void Server::add_channel(Channel & channel) {
		this->_channel_list.push_back(&channel);
	}
	void Server::del_channel(Channel & channel) { // WIP need to delete user chan list and chanstatus
		for (std::vector<Channel *>::iterator it = this->_channel_list.begin(); it != this->_channel_list.end(); ++it) {
			if (*it == &channel) {
				this->_channel_list.erase(it);
				break;
			}
		}
	}
	void Server::del_user(User & user) { // WIP need to delete user from channel before delete user
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); ++it) {
			if (*it == &user) {
				this->_user_list.erase(it);
				break;
			}
		}
	}
	void Server::exec_cmd(User & user, irc::command *command) {
		(void)user;
		(void)command;
		// command->get_args();
		// command->exec_cmd(const command &cmd);
	}
	User * Server::check_user_existance(User & user) {
		for( vec_cit_user it = this->_user_list.begin(); it != this->_user_list.end(); ++it) {
			if ((*it)->get_uuid() == user.get_uuid()) {
					if ((*it)->get_socket() != NULL)
						return *it; // user already exists in the server and is connected
					else {
						(*it)->set_socket(user.get_socket());
						this->del_user(user);
						return NULL;
					}
			}
		}
		return NULL;
	}
}
