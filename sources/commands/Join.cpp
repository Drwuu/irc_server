#include "../../headers/commands/Join.hpp"

namespace irc {
/* Constructors & Destructors */
	Join::~Join() {};
	Join::Join(){
		_args.push_back("Join");
	};
/* Operators */
/* Functions */
	void Join::exec_cmd(command const &cmd, User const &user) const {
		(void)cmd;
		(void)user;
	};

	void Join::is_valid_args(Server const *Server, User const &user) const {
		if (_args.size() < 2)
			throw error(_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);
		vec_user const users = Server->get_user_list();
		if (users.size() == 0 || Server->find_nickname(_args[1], users) == users.end())
			throw error(_args[1] + ": No such nick", ERR_NOSUCHNICK);
		vec_chan const chans = Server->get_channel_list();
		vec_cit_chan mchan = Server->find_chan_name(_args[2], chans);
		if (chans.size() == 0 || mchan == chans.end())
			throw error(_args[2] + ": No such channel", ERR_NOSUCHNICK);
	};
}
