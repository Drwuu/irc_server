#pragma once
#include "../error/error.hpp"
#include "../commands/command.hpp"
#include "../parser/parser.hpp"
#include "../proxy/Proxy_queue.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <list>

namespace irc {
	class Server {
	/* Typedefs */
		private:
			// Events should be allowed to use Server private elements
			// to modify it deeply
			friend struct irc::Server_queue;
		public:
	/* Variables */
		private:
			std::string					_name;
			std::string					_label;
			std::string					_hostname;
			std::string					_password;
			int							_port;
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
			parser						_parser;
			std::list<Socket_event *>	_event_list;
		public:
	/* Constructors & Destructors */
		private:
			Server(Server const &copy);
		public:
			Server();
			Server(std::string password, int port);
			~Server();
	/* Operators */
		private:
		public:
			Server& operator=(Server const &src);
	/* Getters */
		private:
		public:
			int 							get_port() const;
			std::string const				get_name() const;
			std::string const				get_label() const;
			std::string const				get_hostname() const;
			std::string const				get_password() const;
			std::string const				get_ip() const;
			std::string const				get_motd() const;
			std::vector<User *>				get_user_list() const;
			std::vector<Channel *> const	get_channel_list() const;
			map_cmd const					get_map() const;
			std::list<Socket_event *>		&get_event_list();

	/* Setters */
		private:
		public:
			void							set_name(std::string name);
			void							set_label(std::string label);
			void							set_hostname(std::string hostname);
			void							set_password(std::string password);
			void							set_port(int port);
			void							set_ip(std::string ip);
			void  							set_motd(std::string motd); // Use For IMPORTMOTD
			void							set_line(std::string line);



	/* Functions */
		private:
		public:
			vec_cit_user 					find_nickname(std::string const &nick, vec_user const &users) const;
			vec_cit_user 					find_username(std::string const &username, vec_user const &users) const;
			vec_cit_chan 					find_chan_name(std::string const &chan, vec_chan const &channels) const;
			User *							get_user_from_socket(Socket<Address_ipv4> const *sock);

			command							*parse_line(User const &user) const;
			void							exec_cmd(User & user, command *command);
			User *							check_user_existance(User & user);

			void							receive_api(std::list<Socket_event *> &api);

			void							add_user(User * user);
			void							add_channel(Channel *channel);
			int								del_user(User & user);
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
