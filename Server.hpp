#ifndef SERVER_HPP
#define SERVER_HPP


#include "Irc.hpp"
#include "User.hpp"
#include "Channel.hpp"
class Server
{
	private:
		std::string				_name;
		std::string				_label;
		std::string				_hostname;
		std::string				_password;
		std::string				_port;
		std::string				_ip;
		std::vector<User *>		_user_list;
		std::vector<Channel *>	_channel_list;
		Server(Server const & copy);
		Server & operator=(Server const & op);
	protected:
		/*Arg*/
	public:
		Server();
		Server(std::string password, std::string port);
		~Server();

		const std::string		get_name() const;
		const std::string		get_label() const;
		const std::string		get_hostname() const;
		const std::string		get_password() const;
		const std::string		get_port() const;
		const std::string		get_ip() const;
		std::vector<User *>		get_user_list();
		std::vector<Channel *>	get_channel_list();

		void					set_name(std::string name);
		void					set_label(std::string label);
		void					set_hostname(std::string hostname);
		void					set_password(std::string password);
		void					set_port(std::string port);
		void					set_ip(std::string ip);

		void					add_user(User & const user);
		void					add_channel(Channel & const channel);

		void					del_user(User & user);
		void					del_channel(Channel & const channel);
};


#endif
