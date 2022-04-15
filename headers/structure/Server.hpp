#ifndef SERVER_HPP
#define SERVER_HPP


#include "../commands/invite.hpp"
#include "../commands/kick.hpp"
#include "../commands/mode.hpp"
#include "../irc.hpp"
#include "Channel.hpp"
#include "../parser/parser.hpp"

class Server
{
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
		irc::map_cmd				_map;
		irc::command				*_cmd;


		Server(Server const & copy);
	protected:
		/*Arg*/
	public:
		Server();
		Server(std::string password, std::string port);
		~Server();

		const std::string				get_name() const;
		const std::string				get_label() const;
		const std::string				get_hostname() const;
		const std::string				get_password() const;
		const std::string				get_port() const;
		const std::string				get_ip() const;
		const std::string				get_motd() const;
		std::vector<User *>				get_user_list();
		const std::vector<Channel *>	get_channel_list() const;
		irc::map_cmd					&get_map();


		void					set_name(std::string name);
		void					set_label(std::string label);
		void					set_hostname(std::string hostname);
		void					set_password(std::string password);
		void					set_port(std::string port);
		void					set_ip(std::string ip);
		void  					set_motd(std::string motd); // Use For IMPORTMOTD


		void					parse_line(void);


		void					add_user(User & user);
		void					add_channel(Channel & channel);
		void					del_user(User & user);
		void					del_channel(Channel & channel);

		void					ban_user(std::string nick);//Use For UNKLINE unban USer from server
		void					unban_user(std::string nick);

		void					print_link(); // Use For LINKS Command
		void					print_chan(std::string hint,std::string arg1,std::string arg2); // Use For LIST Command | Syntaxe : /list [#chaine] [-min #] [-max #]
		void					print_registered_user(std::string servername); // Use FOR LIST Command | Syntaxe : /lusers [nomserveur]
		void					print_channel_user(std::string channel); // Use FOR NAMES Command | Syntaxe : /lusers [nomserveur]
		void					print_motd(); // Use For MOTD Command

		void					exec_cmd(User & user, irc::command *command);
};


#endif
