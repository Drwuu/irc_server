#ifndef USER_HPP
#define USER_HPP

#include "Irc.hpp"

class User
{
	private:
		std::string _username;
		std::string _nickname;
		std::string _password;
		std::string _ip;
		std::string _port;
		std::string _uuid;
		std::string _mode;
		std::vector<std::string> _past_username;
		std::vector<t_chanstatus> _chan_list;
		User(User const & copy);
		User & operator=(User const & op);

	protected:
		/*Arg*/
	public:
		User();
		~User();

		const std::string			get_username() const;
		const std::string			get_nickname() const;
		const std::string			get_password() const;
		const std::string			get_uuid() const;
		const std::string			get_port() const;
		const std::string			get_ip() const;
		const std::string			get_mode() const;
		std::vector<std::string>	get_past_username();
		std::vector<t_chanstatus>	get_chan_list();

		void						set_password(std::string password);
		void						set_ip(std::string ip);
		void						set_port(std::string port);
		void						set_uuid(std::string uuid);
		void						set_username(std::string username);
		void						set_nickname(std::string nickname);
};
#endif
