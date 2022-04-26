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
			std::string _ip; // Not in use
			int _port; // Not in use
			std::string _uuid; // Not in use
			std::string _mode;	// Not in use | Use bool for usermode and chanstatus
			std::vector<std::string> _past_username; // Not in use
			std::vector<ChanStatus> _chan_list;
			irc::map_cmd * _cmd_list;
			std::list<Socket_event *> _event_list;
			bool _is_away;
			bool _is_pass_checked;
			bool _is_registered;
			bool _is_irc_operator; // FIXME : REMOVE
			Socket<Address_ipv6> const	*_socket;
			User(User const & copy);
			Server * _server; //TEMP

		protected:
			std::vector<ChanStatus>::iterator	get_chan_status(const Channel *channel);

		public:
			User();
			User(Socket<Address_ipv6> const *socket);
			~User();

			int								get_port() const; // Not in use
			const std::string				get_nickname() const;
			const std::string				get_username() const;
			const std::string				get_hostname() const;
			const std::string				get_servername() const;
			const std::string				get_realname() const;
			const std::string				get_uuid() const; // Not in use
			const std::string				get_ip() const; // Not in use
			const std::string				get_mode() const; // Not in use
			bool							get_operator_status(const Channel *channel) const;
			bool							get_registered_status()const ;
			bool							get_password_status() const;
			bool							get_away_status() const;
			std::vector<std::string>		get_past_username();
			std::vector<ChanStatus>			get_chan_list();
			const std::vector<ChanStatus>	get_chan_list() const;
			std::vector<ChanStatus>::const_iterator		get_chanstatus_from_list(Channel const & channel,std::vector<ChanStatus> const &chans) const;
			const Socket<Address_ipv6>		*get_socket() const;

			Server * get_server();
			void set_server(Server * server);
			std::list<Socket_event *>		get_event_list();
			void set_event_list(std::list<Socket_event *> &event_list);

			Channel							*find_channel(std::string const &name) const;

			void						set_ip(std::string ip);
			void						set_port(int port);
			void						set_uuid();
			void						set_username(std::string username);
			void						set_realname(std::string realname);
			void						set_hostname(std::string hostname);
			void						set_nickname(std::string nickname); // Command NICK
			void						set_socket(Socket<Address_ipv6> const *socket);
			void						set_registered_status(bool status);
			void						set_password_status(bool status);
			void						set_chan_status(const Channel *channel, bool op);
			void						set_mute(const Channel *channel, bool value);
			void						join_channel(ChanStatus &status); // Command JOIN
			void						leave_channel(std::string channel); // Command PART
			void						leave_channel(const Server & Server, Channel *channel);// Command PART
			bool						is_mute(const Channel *channel);

			void						send_message(Server & Server, Channel & Channel,std::string msg); // Command MSG et/ou PRIVMSG
			void						send_message(std::string msg, User & User); // Command MSG et/ou PRIVMSG
			void						send_message(std::string msg, Channel & channel);
			void						receive_message(Server & Server,	Channel& channel,std::string msg){(void)Server;(void)channel;(void)msg;};
			void						receive_message(User * user,std::string msg);
			void						receive_message(Server & Server,User& user, std::string msg){(void)Server;(void)user;(void)msg;};

			void						send_invite(User & user, Channel & channel);
			void						receive_invite(Channel & channel);

			void						kick_user(User & user, Channel & channel,std::string msg); // Command Kick Need to test is this is op
			void						ban_user(User & user, Channel & channel); // Use /mode #channel + b pseudo
			void						unban_user(User & user, Channel & channel); // Use Command UNBAN
			void						unmute_user(User & user, Channel & Channel); // Use /mode username -v Authorize talking in moderated channel (mode m)
			void						change_topic(Channel & channel,std::string msg); // Used for TOPIC function
			int							disconnect_user(string const msg_quit);
};
}
