#include "../../headers/commands/Part.hpp"

namespace irc {
/* Constructors & Destructors */
	Part::~Part() {};
	Part::Part(){};
	Part::Part(Server *_server): command(_server) {};

	bool Part::is_valid_channel(const std::string &channel) const{
		if (channel[0] != '#' and channel[0] != '&' and channel[0] != '+' and channel[0] != '!')
			return false;
		if (channel.size() > 50)
			return false;
		for (std::string::const_iterator it = channel.begin(); it != channel.end();++it)
		{
			if (*it == 0 or *it == 7 or *it == 10 or *it == 13 or *it == 32 or *it == 44 or *it == 58)
				return false;
		}
		return true;
	}

	void	Part::exec_cmd(User &user ) {
		std::vector<ChanStatus> chanlist = user.get_chan_list();
		for (std::vector<std::string>::iterator it = _chan_list.begin(); it != _chan_list.end(); ++it)
		{
			std::vector<ChanStatus>::const_iterator chanstatus = user.get_chanstatus_from_list(*it, chanlist);
			if (chanstatus != chanlist.end()){
				std::string ret = ":" + user.get_nickname() + " " + _args[0] + " " + chanstatus->channel->get_name() + "\r\n";
				Proxy_queue::Write * msg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
				_server->get_event_list().push_back(msg);
				chanstatus->channel->transmit_message( " PART " + chanstatus->channel->get_name(), &user);
				if (chanstatus->channel->get_user_list().size() == 1)
				{
					_server->del_channel(*chanstatus->channel);
				}
				user.leave_channel(*it);
			}
			else
			{
				std::string ret = ":" + _server->get_name() + " 403 " + user.get_nickname() + " " + *it + " :No such channel\r\n";
				Proxy_queue::Write * msg = new Proxy_queue::Write(user.get_socket()->get_fd(),ret.c_str());
				_server->get_event_list().push_back(msg);
			}
			chanlist = user.get_chan_list();
		}
		_chan_list.clear();
	}

	bool	Part::is_valid_args(User const &user) {
		_chan_list.clear();
		if (_args.size() < 2)
			throw error("PART : Not enough arguments", ERR_NEEDMOREPARAMS);
		std::string tmp = _args[1];
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = tmp.find(',', prev)) != std::string::npos) {
			_chan_list.push_back(tmp.substr(prev, pos - prev));
			prev = pos + 1;
		}
		_chan_list.push_back(tmp.substr(prev));
		for(std::vector<std::string>::iterator it = _chan_list.begin(); it != _chan_list.end(); ++it)
		{
			if (!is_valid_channel(*it))
			{
				std::clog << "Channel name invalid = " << *it << std::endl;
				throw error("PART : Invalid channel name", ERR_NOSUCHCHANNEL);
			}
			std::vector<Channel *> chanlist = _server->get_channel_list();
			std::vector<ChanStatus> chanstatuslist = user.get_chan_list();
			std::vector<ChanStatus>::const_iterator chanstatus;
			if (_server->find_chan_name(*it, chanlist) == chanlist.end())
				throw error("PART : Channel not found", ERR_NOSUCHCHANNEL);
			if ((chanstatus = user.get_chanstatus_from_list(*it, chanstatuslist)) == chanstatuslist.end())
				throw error("PART : You are not in this channel", ERR_NOTONCHANNEL);
		}
		return true;
	}
}
