#include "../../headers/commands/Message.hpp"

namespace irc {
/* Constructors & Destructors */
	Privmsg::~Privmsg() {};
	Privmsg::Privmsg(){};
	Privmsg::Privmsg(Server *server): command(server) {};

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

	// Big problem : std::string 'nickname' is not used :P
	// I think this is a malfunction or an error
	bool Privmsg::is_valid_nickname(const std::string &) const{
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

	void	Privmsg::exec_cmd(User &user ) {
		//if (is_valid_channel(this->get_args()[1]))
		//	 user.send_message(server->find_chan_name(this->get_args()[1], server->get_channel_list()),this->get_args()[2]);
		//if (is_valid_nickname(this->get_args()[1]))
		std::vector<User *>::const_iterator receiver = _server->find_nickname(this->get_args()[1], _server->get_user_list());
		if (receiver != _server->get_user_list().end())
		{
			std::clog << "-------------EXECUTION COMMANDE PRIVMSG" << std::endl;
			std::clog << "Message From : " << user.get_nickname() << "to : " << (*receiver)->get_nickname() << std::endl;
			user.send_message(this->get_args()[2], *(*receiver));
		}
		std::cout << _args[2] << std::endl;
	}

	void	Privmsg::is_valid_args(Server const *Server, User const &user) {
		// Possible numeric reply ERR_NORECIPIENT ERR_NOTEXTTOSEND ERR_CANNOTSENDTOCHAN
		//ERR_NOTOPLEVEL ERR_WILDTOPLEVEL ERR_TOOMANYTARGETS ERR_NOSUCHNICK RPL_AWAY
		// WIP mask a gere Oui ? Non
		if (user.get_registered_status() == false)
			throw error("You must be registered to use this command",ERR_NOTREGISTERED );
		if (_args.size() < 3)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		else if (_args.size() < 4 && _args[2].size() == 0)
			throw error(_args[0] + ": No text to send", ERR_NOTEXTTOSEND);
		if (!is_valid_receiver(_args[1]))
			throw error("No recipient given", ERR_NORECIPIENT);
		if (!find_receiver(Server,this->_args[1]))
			throw error(_args[1],ERR_NOSUCHNICK);

		// FIXME : This is a bit shady. Maybe `is_authorized()` should be authorized to take
		// a `const Channel *` instead of a reference.
		if (is_valid_channel(_args[1])) {
			irc::vec_cit_chan	it = Server->find_chan_name(_args[1], Server->get_channel_list());
			if (!is_authorized(*(*it), user)) // <- HERE
				throw error("You are not authorized to send messages to this channel", ERR_CANNOTSENDTOCHAN);
		}
	}
}
