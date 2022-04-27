#include "../../headers/commands/invite.hpp"

namespace irc {
/* Constructors & Destructors */
	invite::~invite() {};
	invite::invite() {};
	invite::invite(Server *server): command(server) {};
/* Operators */
/* Functions */
	void invite::exec_cmd(User &user) {
		vec_chan const servChans = _server->get_channel_list();
		vec_cit_chan mchan = _server->find_chan_name(_args[2], servChans);
		_server->find_username(_args[1], _server->get_user_list());
		(*mchan)->invite_user(_args[1]);
		std::stringstream s;
		if (mchan != servChans.end()) {
			s << ":" << _server->get_name() << " " << RPL_INVITING << " "
			<< user.get_nickname() << " " << _args[1] << " " << (*mchan)->get_name() << "\r\n\0";
			Proxy_queue::Write *msg = new Proxy_queue::Write(user.get_socket()->get_fd(), s.str().c_str());
			_server->get_event_list().push_back(msg);

			vec_user servUsers =  _server->get_user_list();
			vec_cit_user it = _server->find_nickname(_args[1], servUsers);
			s << ":" << user.get_nickname() << " INVITE " << _args[1] << " :" << (*mchan)->get_name() << "\r\n\0";
			Socket<Address_ipv4> const *sock = (*it)->get_socket();
			msg = new Proxy_queue::Write(sock->get_fd(), s.str().c_str());
			_server->get_event_list().push_back(msg);
		}
	};

	bool invite::is_valid_args(User const &user) {

		/* Throw error not enough parameters */
		if (_args.size() < 3)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);

		/* Throw error no such channel */
		vec_chan servChans = _server->get_channel_list();
		vec_cit_chan const mchan = _server->find_chan_name(_args[2], servChans);
		if (mchan == servChans.end())
			throw error(_args[2] + " :No such channel", ERR_NOSUCHCHANNEL);

		/* User that invite is not on channel */
		vec_user const chanUsers = (*mchan)->get_user_list();
		vec_cit_user const muser = _server->find_nickname(user.get_nickname(), chanUsers);
		if (muser == chanUsers.end())
			throw error(user.get_nickname() + " :You're not on that channel", ERR_NOTONCHANNEL);
		std::vector<ChanStatus> userChans = (*muser)->get_chan_list();

		/* Throw error already on channel */
		if (chanUsers.size() != 0 && _server->find_nickname(_args[1], chanUsers) != chanUsers.end())
			throw error(_args[1] + " :is already on channel " + _args[2], ERR_USERONCHANNEL);

		/* User that invite is not channel operator */
		std::vector<ChanStatus>::const_iterator userStatut = (*muser)->get_chanstatus_from_list(**mchan, userChans);
		if (!userStatut->is_operator)
			throw error(user.get_nickname() + " :You're not channel operator", ERR_CHANOPRIVSNEEDED);

		/* Throw error on nick and channel */
		vec_user const servUsers = _server->get_user_list();
		if (servUsers.size() == 0 || _server->find_nickname(_args[1], servUsers) == servUsers.end())
			throw error(_args[1] + " :No such nick", ERR_NOSUCHNICK);

		return true;
	};
}
