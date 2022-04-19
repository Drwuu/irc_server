#pragma once
#include "Server.hpp"
#include <list>

namespace irc {
	class Server;
	class Channel;
	struct ChanStatus
	{
		ChanStatus(Channel * channel);
		Channel *channel;
		bool is_admin;
		bool is_banned;
		bool is_mute;
		bool is_operator;
	};
	class User
	{
		private:
//      Command: USER   Parameters: <username> <hostname> <servername> <realname>
			std::string _nickname; // nickname Used For NICK command
			std::string _username; // username Used For USER command
			std::string _hostname; // hostname Used For USER command
			std::string _servername; // servername Used For USER command
			std::string _realname; // realname Used For USER command
			std::string _password; // password Used For PASS command
			std::string _ip; // Not in use
			std::string _port; // Not in use
			std::string _uuid; // Not in use
			std::string _mode;	// Not in use | Use bool for usermode and chanstatus
			std::vector<std::string> _past_username; // Not in use
			std::vector<ChanStatus> _chan_list;
			irc::map_cmd * _cmd_list;
			std::list<Socket_event *> _event_list;
			bool _is_away;
			bool _is_registered;
			bool _is_irc_operator;
			Socket<Address_ipv6>	 *_socket;
			User(User const & copy);

		protected:
			/*Arg*/
		public:
			User();
			User(Socket<Address_ipv6> * socket);
			~User();

			const std::string				get_nickname() const;
			const std::string				get_username() const;
			const std::string				get_hostname() const;
			const std::string				get_servername() const;
			const std::string				get_realname() const;
			const std::string				get_password() const;
			const std::string				get_uuid() const; // Not in use
			const std::string				get_port() const; // Not in use
			const std::string				get_ip() const; // Not in use
			const std::string				get_mode() const; // Not in use
			bool							get_operator_status() const;
			bool							get_registered_status()const ;
			bool							get_away_status() const;
			std::vector<std::string>		get_past_username();
			std::vector<ChanStatus>			get_chan_list();
			const std::vector<ChanStatus>	get_chan_list() const;
			ChanStatus						*get_chanstatus_from_list(Channel * channel);
			const ChanStatus				*get_chanstatus_from_list(Channel * channel) const;
			Socket<Address_ipv6>			*get_socket();

			void						set_password(std::string password);
			void						set_ip(std::string ip);
			void						set_port(std::string port);
			void						set_uuid();
			void						set_username(std::string username);
			void						set_nickname(std::string nickname); // Command NICK
			void						set_socket(Socket<Address_ipv6> * socket);
			void						join_channel(Server & Server, Channel * channel); // Command JOIN
			void						join_channel(Server & Server, std::string channel, std::string key); // Command JOIN with Key mode
			void						leave_channel(Channel * channel);
			void						leave_channel(const Server & Server, Channel *channel);// Command PART

			void						send_message(Server & Server, Channel & Channel,std::string msg); // Command MSG et/ou PRIVMSG
			void						send_message(Server & Server, User & User, std::string msg); // Command MSG et/ou PRIVMSG
			void						send_message(std::string msg,Channel & channel);
			void						receive_message(Server & Server,	Channel& channel,std::string msg){(void)Server;(void)channel;(void)msg;};
			void						receive_message(User * user,std::string msg);
			void						receive_message(Server & Server,User& user, std::string msg){(void)Server;(void)user;(void)msg;};

			void						send_invite(User & user, Channel & channel);
			void						receive_invite(Channel & channel);

			void						kick_user(User & user, Channel & channel,std::string msg); // Command Kick Need to test is this is op
			void						ban_user(User & user, Channel & channel); // Use /mode #channel + b pseudo
			void						unban_user(User & user, Channel & channel); // Use Command UNBAN
			void						op_user(User & user, Channel & channel); // Use /mode #channel -o pseudo
			void						unmute_user(User & user, Channel & Channel); // Use /mode username -v Authorize talking in moderated channel (mode m)
			void						change_topic(Channel & channel,std::string msg); // Used for TOPIC function
			int							disconnect_user();
};
}

