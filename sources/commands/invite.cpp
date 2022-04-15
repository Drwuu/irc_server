#include "../../headers/commands/invite.hpp"
#include "../../headers/error/error.hpp"

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
	void invite::is_valid_args() const {
		if (_args.size() < 3)
			throw error(string(_args[0]) + ": Not enough parameters", ERR_NEEDMOREPARAMS);
		// if ()
		// 	throw error(args[1] + ": No such nick/channel", ERR_NOSUCHNICK);
		// if (...)
			// throw error(args[2] + ": You're not on that channel", ERR_NOTONCHANNEL);
		// if (...)
			// throw error(args[1] + " " + args[2] + ": is already on channel", ERR_USERONCHANNEL);
		// if (...)
			// throw error(args[2] + ": You're not channel operator", ERR_CHANOPRIVSNEEDED);
		// if (...)
			// throw error(args[2] + " " + args[1], RPL_INVITING);
		// if (...)
			// throw error(args[1] + ": <message d'absence>" + args[1], RPL_AWAY);


	};
}
