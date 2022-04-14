#ifndef USER_HPP
#define USER_HPP

#include "../irc.hpp"
#include "Channel.hpp"

class Server;

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
		std::string _username;
		std::string _nickname;
		std::string _password;
		std::string _ip;
		std::string _port;
		std::string _uuid;
		std::string _mode;
		std::vector<std::string> _past_username;
		std::vector<ChanStatus> _chan_list;
		irc::map_cmd * _cmd_list;
		bool _is_away;
		bool _is_registered;
		bool _is_irc_operator;
		User(User const & copy);

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
		bool					get_operator_status() const;
		bool					get_registered_status()const ;
		bool					get_away_status() const;
		std::vector<std::string>	get_past_username();
		std::vector<ChanStatus>		get_chan_list();
		ChanStatus	*				get_chanstatus_from_list(Channel * channel);

		void						set_password(std::string password);
		void						set_ip(std::string ip);
		void						set_port(std::string port);
		void						set_uuid(std::string uuid);
		void						set_username(std::string username);
		void						set_nickname(std::string nickname); // Command NICK (optionnel)

		void						join_channel(Server & server, std::string channel); // Command JOIN
		void						join_channel(Server & server, std::string channel, std::string key); // Command JOIN with Key mode
		void						leave_channel(std::string channel);
		void						leave_channel(const Server & server, Channel *channel);// Command PART

		void						send_message(Server & server, Channel & Channel,std::string msg); // Command MSG et/ou PRIVMSG
		void						send_message(Server & Server, User & User, std::string msg); // Command MSG et/ou PRIVMSG
		void						receive_message(Server & server,	Channel& channel,std::string msg){(void)server;(void)channel;(void)msg;};
		void						receive_message(Server & Server,User& user, std::string msg){(void)Server;(void)user;(void)msg;};
		void						send_invite(User & user, Channel & channel);
		void						receive_invite(User & user, Channel & channel);

		void						kick_user(User & user, Channel & channel,std::string msg); // Command Kick Need to test is this is op
		void						ban_user(User & user, Channel & channel); // Use /mode #channel + b pseudo
		void						unban_user(User & user, Channel & channel); // Use Command UNBAN
		void						op_user(User & user, Channel & channel); // Use /mode #channel -o pseudo
		void						unmute_user(User & user, Channel & Channel); // Use /mode username -v Authorize talking in moderated channel (mode m)
		void						change_topic(Channel & channel,std::string msg); // Used for TOPIC function
};
#endif
