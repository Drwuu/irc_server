#include "../../headers/commands/Join.hpp"
#include <vector>

namespace irc {
/* Constructors & Destructors */
	Join::~Join() {};
	Join::Join() {};
/* Operators */
/* Functions */
	void Join::exec_cmd(User &user) {
		(void)user;
	};

	void Join::is_valid_args(Server const *Server, User const &) const {
		if (_args.size() < 2)
			throw error(_args[0] + ": Not enough parameters", ERR_NEEDMOREPARAMS);

		vec_chan const serv_chans = Server->get_channel_list();
		vec_cit_chan mchan = Server->find_chan_name(_args[1], serv_chans);
		if (serv_chans.size() == 0 || mchan == serv_chans.end())
			throw error(_args[1] + ": No such channel", ERR_NOSUCHCHANNEL);

		// continue here

		// ChanStatus *chanStatut = user.get_chanstatus_from_list();
		// if (...)
		// 	throw error(_args[1] + ": Cannot join channel (+b)", ERR_BANNEDFROMCHAN);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", ERR_INVITEONLYCHAN);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", ERR_BADCHANNELKEY);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", ERR_CHANNELISFULL);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", ERR_BADCHANMASK);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", ERR_TOOMANYCHANNELS);
		// if (...)
			// throw error(_args[0] + ": Not enough parameters", RPL_TOPIC);
	};
}
