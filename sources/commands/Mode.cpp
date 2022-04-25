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

/* Constructors & Destructors */
	Mode::~Mode() {};
	Mode::Mode() {};
	Mode::Mode(Server *server)
		: command(server), _modes(), _modes_args(), _sign(0) {};

/* Operators */
/* Functions */
	void Mode::exec_cmd(User &) {
		if (_args[1][0] == '#' || _args[1][0] == '&')
			_exec_chanMode();
		else
			_exec_userMode();
	};

	//// How to valid arguments :
	// 1 - check if channel exists
	// 2 - check if option exists
	// 3 - check privileges
	// 4 - check if is in channel
	//// Pending
	// 5 - check if double letters
	// 6 - check if enough arguments
	bool Mode::is_valid_args(User const &user) {
		if (_args.size() < 3)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);
		else if (_args[1][0] == '#' || _args[1][0] == '&')
			_valid_chanMode(user);
		else
			_valid_userMode(user);
		return true;
	};


	/* Calculate the number of arguments required to make MODE works */
	bool 	Mode::_is_valid_arg_nb(string const &modes) {
		size_t	mode_nb = 0;

		for (string::const_iterator it = modes.begin() ; it != modes.end() ; ++it) {
			// If the flag require an argument
			if (*it == 'o' || *it == 'l' || *it == 'b' || *it == 'k')
				++mode_nb;
		}
		return !(mode_nb > _args.size() - 3);
	}

	//// Channel mode errors
	// ERR_NEEDMOREPARAMS	-	ERR_CHANOPRIVSNEEDED	-	ERR_NOTONCHANNEL
	// ERR_UNKNOWNMODE		-	ERR_KEYSET				-	(ERR_NOSUCHNICK)->implemented elsewhere
	//
	void Mode::_valid_chanMode(User const &user) {
		vec_chan const serv_chans = _server->get_channel_list();
		vec_cit_chan mchan = _server->find_chan_name(_args[1], serv_chans);
		if (serv_chans.size() == 0 || mchan == serv_chans.end())
			throw error(_args[1] + " :No such channel", ERR_NOSUCHCHANNEL);

		string pos;
		if (!_is_valid_mode(_args[2], "opsitnmlbvkw-+", pos))
			throw error(pos + " :is unknown mode char to me", ERR_UNKNOWNMODE);

		if (!_is_valid_arg_nb(_args[2]))
			throw error(" :Not enough parameters", ERR_NEEDMOREPARAMS);

		vec_user opUsers = (*mchan)->get_operator_list();
		vec_cit_user it = _server->find_nickname(user.get_nickname(), opUsers);
		if (it == opUsers.end())
			throw error(_args[1] + " :You're not channel operator", ERR_CHANOPRIVSNEEDED);

		vec_user chanUsers = (*mchan)->get_user_list();
		vec_cit_user it2 = _server->find_nickname(user.get_nickname(), chanUsers);
		if (it2 == chanUsers.end())
			throw error(_args[1] + " :You're not on that channel", ERR_NOTONCHANNEL);
	}

	//// User mode errors :
	//	ERR_UMODEUNKNOWNFLAG
	//	ERR_USERSDONTMATCH
	//
	void Mode::_valid_userMode(User const &user) {
		/* We don't need this part : if the nickname doesn't match with user's one,
		 a ERR_USERSDONTMATCH is thrown.
		 Reason : 'A user MODE command may only be accepted if both the sender of the
		 message and the nickname given as a parameter are both the same.' */
		//
		// vec_user const serv_users = _server->get_user_list();
		// vec_cit_user muser = _server->find_nickname(_args[1], serv_users);
		// if (serv_users.size() == 0 || muser == serv_users.end())
			// throw error(_args[1] + " :No such nick", ERR_NOSUCHNICK);
		
		if (user.get_nickname() != _args[1])
			throw error(" :Cant change mode for other users", ERR_USERSDONTMATCH);

		string pos;
		if (!_is_valid_mode(_args[2], "iwso", pos))
			throw error(pos + " :Unknown MODE flag", ERR_UMODEUNKNOWNFLAG);
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

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//

	bool	Mode::_sign_handler(const char new_sign) {
		if (new_sign != _sign)
			_sign = new_sign;
		return '+' == new_sign;
	}

	void	Mode::_build_return_message(User const &) { }
	// erase repetitive flags
	//+obbobi guhernan2 guhernan3 guhernan3 guhernan2 guhernan2 = +oo guhernan2 guhernan2


	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//
	// CHANNEL MODE
	//
	//  With args
	void	Mode::_channel_mode_o(vector_string::const_iterator arg, const User &author) { // user
		if (author.get_nickname() == *arg)
			return ;

		vec_chan		channel_list = _server->get_channel_list();
		vec_cit_chan 	it_chan = _server->find_chan_name(_args[0], channel_list);
		vec_user		chanop_list = (*it_chan)->get_operator_list();
		User			*target = (*it_chan)->find_user(*arg);

		if (target == NULL)
			throw error(": " + *arg + " No such channel/nick", ERR_NOSUCHNICK);
		if (_sign == '+') {
			if (author.get_operator_status()
					|| std::find(chanop_list.begin(), chanop_list.end(), target) != chanop_list.end())
				return ;
			chanop_list.push_back(target);
			target->set_chan_status(*it_chan, true);
			// set user as chanop --> use a seter TODO
		}
		else {
			if (author.get_operator_status() == false
					|| std::find(chanop_list.begin(), chanop_list.end(), target) == chanop_list.end())
				return ;
			chanop_list.erase(std::find(chanop_list.begin(), chanop_list.end(), target));
			target->set_chan_status(*it_chan, false);
		}

	}
	void	Mode::_channel_mode_l(vector_string::const_iterator arg, const User &author) { // limit -> ONLY  if (is_positive == true)
	}
	void	Mode::_channel_mode_b(vector_string::const_iterator arg, const User &author) { // ban mask
	}
	void	Mode::_channel_mode_k(vector_string::const_iterator arg, const User &author) { // channel key : password
	}

	// No args
	void	Mode::_channel_mode_t(vector_string::const_iterator arg, const User &author) {
	}
	void	Mode::_channel_mode_n(vector_string::const_iterator arg, const User &author) {
	}
	void	Mode::_channel_mode_m(vector_string::const_iterator arg, const User &author) {
	}
	void	Mode::_channel_mode_v(vector_string::const_iterator arg, const User &author) {
	}

	// flag
	void	Mode::_channel_mode_p(vector_string::const_iterator arg, const User &author) {
	}
	void	Mode::_channel_mode_s(vector_string::const_iterator arg, const User &author) {
	}
	void	Mode::_channel_mode_i(vector_string::const_iterator arg, const User &author) {
	}

	void Mode::_exec_chanMode(const User &author) {
		// bool is_positive = false;
		vector_string::const_iterator	it_args = _args.begin() + 3;
		for (std::string::const_iterator it = _args[2].begin() ; it != _args.end() ; ++it) {
			switch (*it) {
				// Sign
				case '+':
					_sign_handler(*it);
					break;
				case '-':
					_sign_handler(*it);
					break;
				// No args needed
				case 't':
					_channel_mode_t(*it_args);
					break;
				case 'n':
					_channel_mode_n(it_args, author);
					break;
				case 'm':
					_channel_mode_m(it_args, author);
					break;
				case 'v':
					_channel_mode_v(it_args, author);
					break;
				case 'p':
					_channel_mode_p(it_args, author);
					break;
				case 's':
					_channel_mode_s(it_args, author);
					break;
				case 'i':
					_channel_mode_i(it_args, author);
					break;
				// Args needed
				case 'o':
					_channel_mode_o(it_args, author);
				case 'l':
					_channel_mode_l(it_args, author);
				case 'b':
					_channel_mode_b(it_args, author);
				case 'k':
					_channel_mode_k(it_args, author);
				default :
					++it_args;
			}
		}

		_build_return_message(author);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// USER MODE

	void	Mode::_user_mode_i(vector_string::const_iterator arg, const User &author) {
	}              u
	void	Mode::_user_mode_s(vector_string::const_iterator arg, const User &author) {
	}              u
	void	Mode::_user_mode_w(vector_string::const_iterator arg, const User &author) {
	}              u
	void	Mode::_user_mode_o(vector_string::const_iterator arg, const User &author) {
	}


	void Mode::_exec_userMode() {
		for (std::string::const_iterator it = _args[2].begin() ; it != _args[2].end() ; ++it) {
			switch (*it) {
				case '+' :
					_sign_handler(*it);
				case '-' :
					_sign_handler(*it);
				case 'i' :
				case 's' :
				case 'w' :
				case 'o' :
			}
		}

		_build_return_message(author);
	}
}
