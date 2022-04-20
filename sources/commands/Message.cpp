#include "../../headers/commands/Message.hpp"

namespace irc {
/* Constructors & Destructors */
	Privmsg::~Privmsg() {};
	Privmsg::Privmsg(){
		_args.push_back("PRIVMSG");
	};

	bool Privmsg::find_receiver(Server const *server,const std::string receiver) const {
		if (server->find_chan_name(receiver, server->get_channel_list()) != server->get_channel_list().end())
			return true;
		else if (server->find_nickname(receiver, server->get_user_list()) != server->get_user_list().end())
			return true;
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
	bool Privmsg::is_valid_nickname(const std::string &nickname) const{
			string::const_iterator it = _args[1].begin();
			if (!(*it >= 65 && *it <= 125))
				return false;
			it++;
			for (;it != _args[1].end();++it)
			{
				if ((*it < 48 && *it != 45) or (*it > 57 and *it < 65) or (*it > 125)) // PTDR CA PUE LE SEXE
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
		const ChanStatus *user_status = user.get_chanstatus_from_list(&channel);
		if (user_status == NULL and channel.is_no_external_msg() == true)
			return false;
		else if (user_status != NULL)
		{
			if (channel.is_moderated() == true && (user_status->is_operator == false or user_status->is_mute == true))
				return false;
			if (user_status->is_banned == true)
				return false;
		}
		return true;
	}

	void	Privmsg::exec_cmd(Server const * server, const command &cmd, User &user) const {
		if (is_valid_channel(cmd.get_args()[1]))
			user.send_message(server->find_chan_name(cmd.get_args()[1], server->get_channel_list()),cmd.get_args()[2]);
		if (is_valid_nickname(cmd.get_args()[1]))
			user.send_message(cmd.get_args()[2],server->find_nickname(cmd.get_args()[1],server->get_user_list()));
	}

	void	Privmsg::is_valid_args(Server const *Server, User const &user)const{
		// Possible numeric reply ERR_NORECIPIENT ERR_NOTEXTTOSEND ERR_CANNOTSENDTOCHAN
		//ERR_NOTOPLEVEL ERR_WILDTOPLEVEL ERR_TOOMANYTARGETS ERR_NOSUCHNICK RPL_AWAY
		// WIP mask a gere Oui ? Non
		if (_args.size() < 3)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		else if (_args.size() < 4 && _args[2].size() == 0)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		if (!is_valid_receiver(_args[1]))
			throw error("No recipient given", ERR_NORECIPIENT);
		if (!find_receiver(Server,this->_args[1]))
			throw error(_args[1],ERR_NOSUCHNICK);
		if (is_valid_channel(_args[1])){
			if (!is_authorized(Server->find_chan_name(_args[1], Server->get_channel_list()), user))
				throw error("You are not authorized to send messages to this channel", ERR_CANNOTSENDTOCHAN);
		}
	}
}
