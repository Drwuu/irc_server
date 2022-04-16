#include "../../headers/commands/invite.hpp"

namespace irc {
/* Constructors & Destructors */
	invite::~invite() {};
	invite::invite(){
		_args.push_back("INVITE");
	};
/* Operators */
/* Functions */
	void invite::exec_cmd(command const &cmd) const {
		(void)cmd;
	};
	void invite::is_valid_args(Server const *Server) const {
		if (_args.size() < 3)
			throw error(_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);

		vec_user const users = Server->get_user_list();
		if (users.size() != 0 && Server->find_nickname(_args[1], users) == users.end())
			throw error(_args[1] + ": No such nick", ERR_NOSUCHNICK);

		vec_chan const chans = Server->get_channel_list();
		vec_cit_chan mchan = Server->find_chan_name(_args[2], chans);
		if (chans.size() != 0 && mchan == chans.end())
			throw error(_args[2] + ": No such channel", ERR_NOSUCHNICK);

		// throw error(_args[1] + ": You're not on that channel", ERR_NOTONCHANNEL);

		vec_user const userChan = (*mchan)->get_user_list();
		if (userChan.size() != 0 && Server->find_nickname(_args[1], userChan) != userChan.end())
			throw error(_args[1] + ": is already on channel " + _args[2], ERR_USERONCHANNEL);
		// if (...)
			// throw error(args[2] + ": You're not channel operator", ERR_CHANOPRIVSNEEDED);
		// if (...)
			// throw error(args[2] + " " + args[1], RPL_INVITING);
		// if (...)
			// throw error(args[1] + ": <message d'absence>" + args[1], RPL_AWAY);


	};
}
