#include "../../headers/structure/server.hpp"
#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/invite.hpp"

namespace irc {
/* Constructors & Destructors */
	server::server(): _line("/invite prout toto"), _map(), _cmd(nullptr) {
		_map.insert(std::make_pair("INVITE", new irc::invite()));
		// _map.insert(std::make_pair("KICK", new irc::kick()));
		// _map.insert(std::make_pair("MODE", new irc::mode()));
		parse_line();
	};
	server::server(std::string password,std::string port): _password(password), _port(port) {
	}
	server::server(server const &src) {
		*this = src;
	};
	server::~server() {
		for (irc::map_iterator_cmd it = _map.begin(); it != _map.end(); it++)
			if (it->second)
				delete it->second;
		_map.clear();
	};

/* Getters */
	irc::map_cmd const server::get_map() const {
		return this->_map;
	};
	std::vector<Channel *> const server::get_channel_list() const {
		return (this->_channel_list);
	}
	std::vector<User *> const server::get_user_list() const {
		return (this->_user_list);
	}
	std::string const server::get_name() const {
		return (this->_name);
	}
	std::string const server::get_hostname() const {
		return (this->_hostname);
	}
	std::string const server::get_password() const {
		return (this->_password);
	}
	std::string const server::get_port() const {
		return (this->_port);
	}
	std::string const server::get_ip() const {
		return (this->_ip);
	}

/* Setters */
	void server::set_name(std::string name){
		this->_name = name;
	}
	void server::set_label(std::string label){
		this->_label = label;
	}
	void server::set_hostname(std::string hostname){
		this->_hostname = hostname;
	}
	void server::set_password(std::string password){
		this->_password = password;
	}
	void server::set_port(std::string port){
		this->_port = port;
	}
	void server::set_ip(std::string ip){
		this->_ip = ip;
	}

/* Functions */
	void server::_init_server(void) {

	};

	void server::parse_line(void) {
		irc::parser parser(_line, _map);
		_cmd = &parser.get_command();
		try {
			_cmd->is_valid_args(this);
		}
		catch (irc::error &e) {
			std::cout << e.what() << std::endl;
		}
	};
	void server::add_user(User & user) {
		this->_user_list.push_back(&user);
	}
	void server::add_channel(Channel & channel) {
		this->_channel_list.push_back(&channel);
	}
	void server::del_channel(Channel & channel) {
		for (std::vector<Channel *>::iterator it = this->_channel_list.begin(); it != this->_channel_list.end(); ++it) {
			if (*it == &channel) {
				this->_channel_list.erase(it);
				break;
			}
		}
	}
	void server::del_user(User & user) {
		for (std::vector<User *>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); ++it) {
			if (*it == &user) {
				this->_user_list.erase(it);
				break;
			}
		}
	}
	void server::exec_cmd(User & user, irc::command *command) {
		(void)user;
		(void)command;
		// command->get_args();
		// command->exec_cmd(const command &cmd);
	}
}
