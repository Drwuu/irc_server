#pragma once
#include "../error/error.hpp"
#include "../commands/command.hpp"
#include "../parser/parser.hpp"
#include "../proxy/Event.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <list>

namespace irc {
	class Server {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			std::string					_name;
			std::string					_label;
			std::string					_hostname;
			std::string					_password;
			std::string					_port;
			std::string					_ip; // Not used
			std::string					_version; // Not used
			std::string					_motd;
			std::string					_date; // Not used
			std::string					_oper_id; // Not used
			std::string					_oper_password; // Not used
			int							_time; // Not used
			std::vector<User *>			_user_list;
			std::vector<Channel *>		_channel_list;
			std::vector<std::string>	_ban_list; // Not used
			std::string					_line;
			map_cmd						_map;
			command						*_cmd;
			std::list<Socket_event *> 	_event_list;
		public:
	/* Constructors & Destructors */
		private:
			Server(Server const &copy);
		public:
			Server();
			Server(std::string password, std::string port);
			~Server();
	/* Operators */
		private:
		public:
			Server& operator=(Server const &src);
	/* Getters */
		private:
		public:
			std::string const				get_name() const;
			std::string const				get_label() const;
			std::string const				get_hostname() const;
			std::string const				get_password() const;
			std::string const				get_port() const;
			std::string const				get_ip() const;
			std::string const				get_motd() const;
			std::vector<User *>				get_user_list() const;
			std::vector<Channel *> const	get_channel_list() const;
			map_cmd const					get_map() const;
	/* Setters */
		private:
		public:
			void							set_name(std::string name);
			void							set_label(std::string label);
			void							set_hostname(std::string hostname);
			void							set_password(std::string password);
			void							set_port(std::string port);
			void							set_ip(std::string ip);
			void  							set_motd(std::string motd); // Use For IMPORTMOTD


	/* Functions */
		private:
		public:
			vec_cit_user const				find_nickname(std::string const &nick, vec_user const &user) const;
			vec_cit_chan const				find_chan_name(std::string const &chan, vec_chan const &channel) const;
			User *							get_user_from_socket(Socket<Address_ipv6> *sock);
			void							parse_line();
			void							exec_cmd(User & user, command *command);

			void							add_user(User * user);
			void							add_channel(Channel & channel);
			void							del_user(User & user);
			void							del_channel(Channel & channel);
			void							ban_user(std::string nick);//Use For UNKLINE unban USer from Server
			void							unban_user(std::string nick);

			void							print_link(); // Use For LINKS Command
			void							print_chan(std::string hint,std::string arg1,std::string arg2); // Use For LIST Command | Syntaxe : /list [#chaine] [-min #] [-max #]
			void							print_registered_user(std::string servername); // Use FOR LIST Command | Syntaxe : /lusers [nomserveur]
			void							print_channel_user(std::string channel); // Use FOR NAMES Command | Syntaxe : /lusers [nomserveur]
			void							print_motd(); // Use For MOTD Command

	};
}
