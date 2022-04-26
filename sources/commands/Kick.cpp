#include "../../headers/commands/Kick.hpp"
#include <utility>

namespace irc {
/* Constructors & Destructors */
	Kick::~Kick() {};
	Kick::Kick(){};
	Kick::Kick(Server *server): command(server) {};
/* Operators */
/* Functions */
	void	Kick::exec_cmd(User &user ) { // :mhaman!~max@46.231.218.157 KICK #tutu gu :gu // :mhaman!~max@46.231.218.157 KICK #tutu gu :Salut bro tu t'en va
		if (_channel.size() == 1)
		{
			vec_chan chanlist = _server->get_channel_list();
			vec_cit_chan chan = _server->find_chan_name(_channel[0],chanlist);
			for (std::vector<std::string>::const_iterator it = _nickname.begin(); it != _nickname.end(); ++it)
			{
				User * parting_user = (*chan)->find_user(*it);
				std::string ret = ":" + user.get_nickname() + " KICK " + (*chan)->get_name() + " " + parting_user->get_nickname() + " " + _kick_message + "\r\n"; // Add Comment Here i think
				Proxy_queue::Write * selfmsg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
				_server->get_event_list().push_back(selfmsg);
				(*chan)->transmit_message( " KICK " + (*chan)->get_name() + " "+ parting_user->get_nickname(), &user); // Maybe PART instead of KICK here for the client to understand what to do
				parting_user->leave_channel((*chan)->get_name());
				std::clog << "Channel to leave = " << *it << std::endl;
				if ((*chan)->get_user_list().size() == 0){
					_server->del_channel(*(*chan));
				}
			}
		}
		else {
			for (std::vector<std::pair<User *,Channel *> >::iterator it = _chan_user_pair.begin(); it != _chan_user_pair.end();it++)
			{
				User * parting_user = it->second->find_user(it->first->get_nickname());
				std::string ret = ":" + user.get_nickname() + " KICK " + it->second->get_name() + " " + parting_user->get_nickname() + " " + _kick_message + "\r\n"; // Add Comment Here i think
				Proxy_queue::Write * selfmsg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
				_server->get_event_list().push_back(selfmsg);
				it->second->transmit_message( " KICK " + it->second->get_name() + " "+ parting_user->get_nickname(), &user); // Maybe PART instead of KICK here for the client to understand what to do
				parting_user->leave_channel(it->second->get_name());
				if (it->second->get_user_list().size() == 0){
					_server->del_channel(*it->second);
					break;
				}
			}
		}
	}

	void Kick::split_arguments() {
		std::string tmp = _args[1];
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = tmp.find(',', prev)) != std::string::npos) {
			_channel.push_back(tmp.substr(prev, pos - prev));
			prev = pos + 1;
		}
		_channel.push_back(tmp.substr(prev));
		pos = 0;
		prev = 0;
		tmp = _args[2];
		while ((pos = tmp.find(',', prev)) != std::string::npos) {
			_nickname.push_back(tmp.substr(prev, pos - prev));
			prev = pos + 1;
		}
		_nickname.push_back(tmp.substr(prev));
	}

	bool Kick::is_valid_args(User const &user) {
		_channel.clear();
		_nickname.clear();
		_chan_user_pair.clear();
		// POSSIBLE NUMERIC ERROR AND REPLIES ERR_NEEDMOREPARAMS ERR_BADCHANMASK ERR_USERNOTINCHANNEL ERR_CHANOPRIVSNEEDED ERR_NOTONCHANNEL;
		_kick_message = "You have beed kicked from the channel";
		if(_args.size() < 3) {
			throw error(user.get_nickname() + "Not engouth PARAMS", ERR_NEEDMOREPARAMS);
		}
		if (_args.size() == 4) {
			_kick_message = _args[3];
		}
		split_arguments();
		if (_nickname.size() != _channel.size()) {
			if (_channel.size() != 1 )
				throw error(user.get_nickname() + "Need more chans", ERR_NEEDMOREPARAMS);
		}
		for (std::vector<std::string>::iterator it = _channel.begin(); it != _channel.end(); ++it){
			vec_chan tmp = _server->get_channel_list();
			vec_cit_chan chan = _server->find_chan_name((*it),tmp);
			std::clog << "Chan name _channel = " << (*it) << std::endl;
			if (chan == tmp.end()) {
				throw error(user.get_nickname() + "No such channel", ERR_NOSUCHCHANNEL);
			}
			// Test if user is in channel
			if (_channel.size() == 1)
			{
				for (std::vector<std::string>::iterator it2 = _nickname.begin(); it2 != _nickname.end(); ++it2) {
					vec_user tmp = (*chan)->get_user_list();
					for (std::vector<User *>::iterator userit = tmp.begin(); userit != tmp.end(); ++userit) {
						if ((*userit)->get_nickname() == (*it2))
						{
							_chan_user_pair.push_back(std::make_pair((*userit), (*chan)));
							break;
						}
						if (userit == tmp.end())
							throw error((*it2) + "No such nickname", ERR_USERNOTINCHANNEL);
					}
				}
			}
			else {
				vec_user tmp = (*chan)->get_user_list();
				for (std::vector<User *>::const_iterator userit = tmp.begin(); userit != tmp.end(); ++userit) {
					if ((*userit)->get_nickname() == _nickname[it - _channel.begin()])
					{
						_chan_user_pair.push_back(std::make_pair((*userit), (*chan)));
						break;
					}
					if (userit == tmp.end())
						throw error(_nickname[it - _channel.begin()] + "No such nickname", ERR_USERNOTINCHANNEL);
				}
			}
			std::vector<ChanStatus> chanlist = user.get_chan_list();
			std::vector<ChanStatus>::const_iterator chan_status = user.get_chanstatus_from_list(*(*chan), chanlist);
			if (chan_status == chanlist.end()) {
				throw error(user.get_nickname() + "You are not on this channel", ERR_NOTONCHANNEL);
			}
			if (chan_status->is_operator == false)
				throw error(user.get_nickname() + "You are not an operator", ERR_CHANOPRIVSNEEDED);
		}
		return true;
	}
}
