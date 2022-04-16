#pragma once
#include "../error/error.hpp"
#include "../commands/command.hpp"
#include "../parser/parser.hpp"
#include "Channel.hpp"
#include "User.hpp"

namespace irc {
	class server {
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
			std::string					_ip;
			std::string					_version;
			std::string					_motd;
			std::string					_date;
			std::string					_oper_id;
			std::string					_oper_password;

			int							_time;
			std::vector<User *>			_user_list;
			std::vector<Channel *>		_channel_list;
			std::vector<std::string>	_ban_list;
			//std::list<Obj *> 			_obj_list;

			std::string					_line;
			map_cmd						_map;
			command						*_cmd;
			std::list<Socket_event *> 		_event_list;
		public:
	/* Constructors & Destructors */
		private:
			server(server const &copy);
		public:
			server();
			server(std::string password, std::string port);
			~server();
	/* Operators */
		private:
		public:
			server& operator=(server const &src);
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
			vec_cit_user const				find_nickname(string const &nick, vec_user const &user) const;
			vec_cit_chan const				find_chan_name(string const &chan, vec_chan const &channel) const;
			void							parse_line();
			void							exec_cmd(User & user, command *command);

			void							add_user(User & user);
			void							add_channel(Channel & channel);
			void							del_user(User & user);
			void							del_channel(Channel & channel);
			void							ban_user(std::string nick);//Use For UNKLINE unban USer from server
			void							unban_user(std::string nick);

			void							print_link(); // Use For LINKS Command
			void							print_chan(std::string hint,std::string arg1,std::string arg2); // Use For LIST Command | Syntaxe : /list [#chaine] [-min #] [-max #]
			void							print_registered_user(std::string servername); // Use FOR LIST Command | Syntaxe : /lusers [nomserveur]
			void							print_channel_user(std::string channel); // Use FOR NAMES Command | Syntaxe : /lusers [nomserveur]
			void							print_motd(); // Use For MOTD Command

	};
}
