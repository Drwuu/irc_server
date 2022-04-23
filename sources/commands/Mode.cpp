#include "../../headers/commands/Mode.hpp"
#include <sys/_types/_size_t.h>

namespace irc {

//////////////////////////////////////////////////////////////////////////////////
////// MODE content
//
//// Channel mode
// <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
//
// 		o - give/take channel operator privileges;
// 		p - private channel flag;
// 		s - secret channel flag;
// 		i - invite-only channel flag;
// 		t - topic settable by channel operator only flag;
// 		n - no messages to channel from clients on the outside;
// 		m - moderated channel;
// 		l - set the user limit to channel;
// 		b - set a ban mask to keep users out;
// 		v - give/take the ability to speak on a moderated channel;
// 		k - set a channel key (password).
//
//
//// User mode
//
// <nickname> {[+|-]|i|w|s|o}
//
// 		i - marks a users as invisible;
// 		s - marks a user for receipt of server notices;
// 		w - user receives wallops;
// 		o - operator flag.
//
//
//// Implemented errors :
//
// ERR_NEEDMOREPARAMS	-	ERR_CHANOPRIVSNEEDED	-	ERR_NOTONCHANNEL
// ERR_UNKNOWNMODE		-	ERR_USERSDONTMATCH		-	
// ERR_UMODEUNKNOWNFLAG	-	ERR_KEYSET
//
// Need implementation :
//	ERR_KEYSET
//	ERR_USERSDONTMATCH

/* Constructors & Destructors */
	Mode::~Mode() {};
	Mode::Mode() {};
	Mode::Mode(Server *server): command(server), _give_privilege(false) {};

/* Operators */
/* Functions */
	void Mode::exec_cmd(User &) {
		if (_args[1][0] == '#' || _args[1][0] == '&')
			_exec_chanMode();
		else
			_exec_userMode();
	};

	// How to valid arguments :
	// 1 - check if channel exists
	// 2 - check if option exists
	// 3 - check privileges
	// 4 - check if is in channel
	// Pending
	// 5 - check if double letters
	// 6 - check if enough arguments
	bool Mode::is_valid_args(User const &user) {
		if (_args.size() < 3)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);
		if (!_is_valid_flag_mode(_args[2]))
			throw error(":Unknown MODE flag", ERR_UMODEUNKNOWNFLAG);

		if (_args[1][0] == '#' || _args[1][0] == '&')
			_valid_chanMode(user);
		else
			_valid_userMode();

		return true;
	};


	void Mode::_valid_chanMode(User const &user) {
		vec_chan const serv_chans = _server->get_channel_list();
		vec_cit_chan mchan = _server->find_chan_name(_args[1], serv_chans);
		if (serv_chans.size() == 0 || mchan == serv_chans.end())
			throw error(_args[1] + " :No such channel", ERR_NOSUCHCHANNEL);
		string pos;
		if (!_is_valid_mode(_args[2], "opsitnmlbvkw", pos))
			throw error(pos + " :is unknown mode char to me", ERR_UNKNOWNMODE);

		vec_user opUsers = (*mchan)->get_operator_list();
		vec_cit_user it = _server->find_nickname(user.get_nickname(), opUsers);
		if (it == opUsers.end())
			throw error(_args[1] + " :You're not channel operator", ERR_CHANOPRIVSNEEDED);

		vec_user chanUsers = (*mchan)->get_user_list();
		vec_cit_user it2 = _server->find_nickname(user.get_nickname(), chanUsers);
		if (it2 == chanUsers.end())
			throw error(_args[1] + " :You're not on that channel", ERR_NOTONCHANNEL);
	}

	void Mode::_valid_userMode() {
		vec_user const serv_users = _server->get_user_list();
		vec_cit_user muser = _server->find_nickname(_args[1], serv_users);
		if (serv_users.size() == 0 || muser == serv_users.end())
			throw error(_args[1] + " :No such nick", ERR_NOSUCHNICK);
		string pos;
		if (!_is_valid_mode(_args[2], "iwso", pos))
			throw error(pos + " :is unknown mode char to me", ERR_UNKNOWNMODE);
	}

	void Mode::_exec_chanMode() {
	}

	void Mode::_exec_userMode() {
	}
	/*
	*	*RESUME*
	*	_is_valid_flag_mode: check validity of flag '+|-' modes
	*/
	bool Mode::_is_valid_flag_mode(string const &arg) const {
		if (arg[0] != '+' && arg[0] != '-')
			return false;
		return true;
	}
	/*
	*	*RESUME*
	*	_is_valid_mode: check validity of modes
	*	If invalid: update pos param with the char of the invalid char
	*/
	bool Mode::_is_valid_mode(string const &arg, string const &modes, string &pos) const {
		bool is_valid = false;
		for (size_t i = 1; i < arg.size(); i++) {								// skip + or - beginning at i = 1
			for (size_t j = 0; j < modes.size(); j++) {
				if (arg[i] == modes[j]) {
					is_valid = true;
					break ;
				}
			}
			if (!is_valid) {
				pos = arg[i];
				return false;
			}
			is_valid = false;
		}
		return true;
	};
}
