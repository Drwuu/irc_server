#include "../../headers/commands/Message.hpp"

namespace irc {
/* Constructors & Destructors */
	Privmsg::~Privmsg() {};
	Privmsg::Privmsg(){};
	Privmsg::Privmsg(Server *_server): command(_server) {};

	bool Privmsg::find_receiver(Server const *_server,const std::string receiver) {
		std::vector<Channel *> chanlist = _server->get_channel_list();
		std::vector<User *> userlist = _server->get_user_list();
		if (_server->find_chan_name(receiver, chanlist) != chanlist.end())
		{
			return true;
		}
		else if (_server->find_nickname(receiver, userlist) != userlist.end())
		{
			return true;
		}
		return false;

	}
	bool Privmsg::is_valid_channel(const std::string &channel) const {
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

	bool Privmsg::is_valid_nickname(const std::string &receiver) const{
			string::const_iterator it = receiver.begin();
			std::clog << "Receiver : " << receiver << std::endl;
			if (!(*it >= 65 && *it <= 125))
				return false;
			it++;
			for (;it != receiver.end();++it)
			{
				if ((*it < 48 && *it != 45) or (*it > 57 and *it < 65) or (*it > 125))
					return false;
			}
			return true;
	}
	bool Privmsg::is_valid_receiver(const std::string &receiver) const {
		if (is_valid_channel(receiver) or is_valid_nickname(receiver))
			return true;
		return false;
	}
	bool Privmsg::is_authorized(const Channel & channel, const User & user) const {
		std::vector<ChanStatus> chanlist = user.get_chan_list();
		std::vector<ChanStatus>::const_iterator chanstatus = user.get_chanstatus_from_list(channel, chanlist);
		vector_string	list_banned = channel.get_banned_user();

		if (chanstatus == chanlist.end() and channel.is_no_external_msg() == true)
			return false;
		else if (chanstatus != chanlist.end()){
			if (channel.is_moderated() == true && (chanstatus->is_operator == false or chanstatus->is_mute == true))
				return false;
			if (std::find(list_banned.begin(), list_banned.end(), user.get_nickname()) != list_banned.end())
				return false;
		}
		return true;
	}

	void	Privmsg::exec_cmd(User &user ) {
		std::clog << "-------------EXECUTION COMMANDE PRIVMSG" << std::endl;
		for(std::vector<std::string>::iterator it = _receiver.begin(); it != _receiver.end();++it)
		{
			if (is_valid_channel((*it)) == true){
				std::vector<Channel *>		chan_list = _server->get_channel_list();
				std::vector<Channel *>::const_iterator receiver = _server->find_chan_name((*it), chan_list);
				user.send_message(" PRIVMSG " + (*it) + " " + _args[2], *(*receiver));
			}
			else {
				std::vector<User *>		user_list = _server->get_user_list();
				std::vector<User *>::const_iterator receiver = _server->find_nickname((*it), user_list);
				user.send_message(" PRIVMSG " + (*receiver)->get_nickname() + " " + _args[2], *(*receiver));
			}
		}
		_receiver.clear();
	}

	bool	Privmsg::is_valid_args(User const &user) {
		_receiver.clear();
		// Possible numeric reply ERR_NORECIPIENT ERR_NOTEXTTOSEND ERR_CANNOTSENDTOCHAN
		//ERR_NOTOPLEVEL ERR_WILDTOPLEVEL ERR_TOOMANYTARGETS ERR_NOSUCHNICK RPL_AWAY
		// WIP mask a gere Oui ? Non
		if (user.get_registered_status() == false)
			throw error("You must be registered to use this command",ERR_NOTREGISTERED );
		if (_args.size() < 3)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		else if (_args.size() < 4 && _args[2].size() == 0)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		std::string tmp = _args[1];
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = tmp.find(',', prev)) != std::string::npos) {
			_receiver.push_back(tmp.substr(prev, pos - prev));
			prev = pos + 1;
		}
		_receiver.push_back(tmp.substr(prev));
		for(std::vector<std::string>::const_iterator it = _receiver.begin(); it != _receiver.end(); ++it)
		{
			if (!is_valid_receiver((*it)))
				throw error("No recipient given", ERR_NORECIPIENT);
			if (!find_receiver(_server,(*it)))
				throw error((*it),ERR_NOSUCHNICK);
			if (is_valid_channel((*it))) {
				irc::vec_chan chanlist = _server->get_channel_list();
				irc::vec_cit_chan	chan = _server->find_chan_name((*it), chanlist);
				if (chan == chanlist.end())
					throw error("No such channel", ERR_NOSUCHCHANNEL);
				if (!is_authorized(*(*chan), user)) // <- HERE
					throw error("You are not authorized to send messages to this channel", ERR_CANNOTSENDTOCHAN);
			}
		}
		return true;
	}
}
