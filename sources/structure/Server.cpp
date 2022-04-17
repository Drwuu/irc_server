#include "../../headers/structure/Server.hpp"
#include <cstddef>

namespace irc {
/* Constructors & Destructors */
	Server::Server(): _line("/invite toto prout"), _map(), _parser(NULL) {
		_map.insert(std::make_pair("INVITE", new invite()));
		// _map.insert(std::make_pair("KICK", new irc::kick()));
		// _map.insert(std::make_pair("MODE", new irc::mode()));
		parser parser(_line, _map);
		_parser =  &parser;
		User user;
		user.set_nickname("TOTO");
		add_user(user);
		Channel chan("PROUT");
		add_channel(chan);
		chan.add_user(&user);
		// admin.join_channel(*this, "PROUT"); //FIXME : doubled user in chan
		vec_user vec =  get_user_list();
		for (vec_cit_user it = vec.begin(); it != vec.end(); it++)
			dprintf (2, "it =  %s\n", (*it)->get_nickname().c_str());
		vec_user chans =  chan.get_user_list();
		for (vec_cit_user it = chans.begin(); it != chans.end(); it++)
			dprintf (2, "chans =  %s\n", (*it)->get_nickname().c_str());
		parse_command(user);
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

/* Functions */
	vec_cit_user const Server::find_nickname(string const &nick, vec_user const &users) const {
		vec_cit_user it = users.begin();
		size_t i;
		for (; it != users.end(); it++) {
			string nickname = (*it)->get_nickname();
			i = nickname.find(nick);
			if (i != string::npos)
				return it;
		}
		return users.end();
	};
	vec_cit_user const Server::find_username(string const &name, vec_user const &users) const {
		vec_cit_user it = users.begin();
		size_t i;
		for (; it != users.end(); it++) {
			string username = (*it)->get_username();
			i = username.find(username);
			if (i != string::npos)
				return it;
		}
		return users.end();
	};
	vec_cit_chan const Server::find_chan_name(string const &chan, vec_chan const &channel) const {
		vec_cit_chan it = channel.begin();
		size_t i;
		for (; it != channel.end(); it++) {
			string nickname = (*it)->get_name();
			i = nickname.find(chan);
			if (i != string::npos)
				return it;
		}
		return channel.end();
	};

	void Server::parse_command(User const &user) const {
		try {
			_parser->get_command().is_valid_args(this, user);
		}
		catch (irc::error &e) {
			std::cout << e.what() << std::endl;
		}
	};
	void Server::add_user(User & user) {
		this->_user_list.push_back(&user);
	}
	void Server::add_channel(Channel & channel) {
		this->_channel_list.push_back(&channel);
	}
	void Server::del_channel(Channel & channel) {
		for (std::vector<Channel *>::iterator it = this->_channel_list.begin(); it != this->_channel_list.end(); ++it) {
			if (*it == &channel) {
				this->_channel_list.erase(it);
				break;
			}
		}
	}
	void Server::del_user(User & user) {
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
}
