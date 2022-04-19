#include "../../headers/commands/invite.hpp"

namespace irc {
/* Constructors & Destructors */
	invite::~invite() {};
	invite::invite(){
		_args.push_back("INVITE");
	};
/* Operators */
/* Functions */
	void invite::exec_cmd(command const &cmd, User const &user) const {
		(void)cmd;
		(void)user;
	};

	void invite::is_valid_args(Server const *Server, User const &user) const {

	/* System errors */
		/* Throw error not enough parameters */
		if (_args.size() < 3)
			throw error(_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);

		vec_user const serv_users = Server->get_user_list();
		vec_chan const serv_chans = Server->get_channel_list();
		vec_cit_chan mchan = Server->find_chan_name(_args[2], serv_chans);
		if (serv_chans.size() == 0 || mchan == serv_chans.end())
			throw error(_args[2] + ": No such channel", ERR_NOSUCHCHANNEL);

	/* Admin errors */
		/* User that invite is not on channel */
		vec_user const chan_users = (*mchan)->get_user_list();
		if (Server->find_username(user.get_username(), chan_users) == chan_users.end())
			throw error(user.get_username() + ": You're not on that channel", ERR_NOTONCHANNEL);
		if (!user.get_operator_status())
			throw error(user.get_username() + ": You're not channel operator", ERR_CHANOPRIVSNEEDED);

	/* Common errors */
		/* Throw error on nick and channel */
		if (serv_users.size() == 0 || Server->find_nickname(_args[1], serv_users) == serv_users.end())
			throw error(_args[1] + ": No such nick", ERR_NOSUCHNICK);
		/* Throw error already on channel */
		if (chan_users.size() != 0 && Server->find_nickname(_args[1], chan_users) != chan_users.end())
			throw error(_args[1] + ": is already on channel " + _args[2], ERR_USERONCHANNEL);

		// if (...)
			// throw error(args[2] + " " + args[1], RPL_INVITING);
		// if (...)
			// throw error(args[1] + ": <message d'absence>" + args[1], RPL_AWAY);


	};
}
