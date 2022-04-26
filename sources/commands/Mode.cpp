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
		: command(server), _sign('+') {};

/* Operators */
/* Functions */
	void Mode::exec_cmd(User &author) {
		if (_args[1][0] == '#' || _args[1][0] == '&')
			_exec_chanMode(author);
		else
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);
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
		if (_args.size() == 2) {
			return true;
		}
		if (_args.size() < 3)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);
		else if (_args[1][0] == '#' || _args[1][0] == '&')
			_valid_chanMode(user);
		else
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);
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
		if (user.get_operator_status(user.find_channel(_args[1])) == false || it2 == chanUsers.end()) {
			throw error(_args[1] + " :You're not on that channel", ERR_NOTONCHANNEL);
		}
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

	void	Mode::_return_channel_modes(User const &author) {
		Channel *channel = author.find_channel(_args[1]);
		std::stringstream	ss_modes;
		std::stringstream	ss_param;
		ss_modes << "+";
		if (channel->is_limited()) {
			ss_modes << "l";
			ss_param << " " << channel->get_userlimit();
		}
		if (!channel->get_key().empty()) {
			ss_modes << "k";
			ss_param << " " << channel->get_key();
		}
		if (channel->is_topic())
			ss_modes << "t";
		if (channel->is_no_external_msg())
			ss_modes << "n";
		if (channel->is_moderated())
			ss_modes << "m";
		if (channel->is_private())
			ss_modes << "p";
		if (channel->is_secret())
			ss_modes << "s";
		if (channel->is_invite())
			ss_modes << "i";

		std::stringstream	 rpl;
		rpl << ":" << _server->get_name() << " " << RPL_CHANNELMODEIS << " " << author.get_nickname()
			<< " " << channel->get_name() << " " << ss_modes.str() << " " << ss_param.str() << "\r\n\0";
		_server->get_event_list().push_back(new Proxy_queue::Write(author.get_socket()->get_fd(),
					rpl.str().c_str()));
	}


	void	Mode::_build_return_message(Channel *channel, User &user) {
		std::stringstream	rpl;
		std::stringstream	rpl_author;
		for (size_t i = 0 ; i < _args.size() ; ++i)
			rpl << " " << _args[i];
		rpl << "\r\n\0";
		channel->transmit_message(rpl.str(), &user);
		rpl_author << ":" << user.get_nickname() << " " << rpl.str();
		_server->get_event_list().push_back(new Proxy_queue::Write(user.get_socket()->get_fd(), rpl_author.str().c_str()));
	}

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//
	// CHANNEL MODE
	//
	//  With args
	void	Mode::_channel_mode_o(Channel *channel, vector_string::const_iterator arg, const User &author) { // user
		vec_chan		channel_list = _server->get_channel_list();
		User			*target = channel->find_user(*arg);

		if (target == NULL)
			throw error(": " + *arg + " No such channel/nick", ERR_NOSUCHNICK);
		if (_sign == '+') {
			if (author.get_operator_status(channel) || channel->is_operator(target))
				return ;
			channel->add_operator(target);
			target->set_chan_status(channel, true);
		}
		else {
			if (author.get_operator_status(channel) == false
					|| channel->is_operator(target) == false)
				return ;
			channel->del_operator(target);
			target->set_chan_status(channel, false);
		}
	}

	// limit -> ONLY  if (is_positive == true)
	void	Mode::_channel_mode_l(Channel *channel, vector_string::const_iterator arg, const User &) { 
		// Validity of the channel has been tested in _is_valid_arg.
		for (std::string::const_iterator it = arg->begin() ; it != arg->end() ; ++it)
			if (!std::isdigit(*it))
				return ;
		int		new_limit = atoi(arg->c_str());
		if (new_limit > 100 || _sign == '-')
			return ;
		channel->set_userlimit(new_limit);
	}

	// ban mask
	void	Mode::_channel_mode_b(size_t, Channel *channel, vector_string::const_iterator arg, const User &) {
		_sign == '+' ? channel->ban_user(*arg) : channel->unban_user(*arg);

		////////////////////////////////////////////////////////////
		//// BANLIST
		////	-> Code for possible implementation
		//
		// std::string		mode_string = _args[2];
		// std::stringstream rpl;
		// rpl << ":" << user.get_nickname() << " " << RPL_BANLIST << " "
			// << channel->get_name() << " " << *arg << "\r\n\0";
		// _server->get_event_list().push_back(new Proxy_queue::Write(user.get_socket()->get_fd(),
					// rpl.str().c_str()));

		// Find if 'mode b' is the last one on mode string, or at the end position
		// return :End of channel ban list.
		// if (pos == _args[2].find_last_of(_args[2], pos)) {
			// std::stringstream	rpl;
			// rpl << ":" << user.get_nickname() << " " << RPL_ENDOFBANLIST << " "
				// << _args[1] << " :End of channel ban list"<< "\r\n\0";
			// _server->get_event_list().push_back(new Proxy_queue::Write(user.get_socket()->get_fd(),
						// rpl.str().c_str()));
		// }
	}

	// channel key : password
	void	Mode::_channel_mode_k(Channel *channel, vector_string::const_iterator arg, const User &) {
		if (_sign == '+')
			channel->set_key(*arg);
	}

	// Authorise or not to speak on moderated channel
	void	Mode::_channel_mode_v(Channel *channel, vector_string::const_iterator arg, const User &) {
		if (channel->is_moderated() == false)
			return ;
		User	*target = channel->find_user(*arg);
		_sign == '+' ? target->set_mute(channel, false) : target->set_mute(channel, true);
	}

	// No args
	void	Mode::_channel_mode_n(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_external_msg(true) : channel->set_external_msg(false);
	}
	void	Mode::_channel_mode_m(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_moderated(true) : channel->set_moderated(false);
	}

	// flag
	void	Mode::_channel_mode_p(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_private(true) : channel->set_private(false);
	}
	void	Mode::_channel_mode_s(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_secret(true) : channel->set_secret(false);
	}
	void	Mode::_channel_mode_i(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_invite_only(true) : channel->set_invite_only(false);
	}
	void	Mode::_channel_mode_t(Channel *channel, vector_string::const_iterator, const User &) {
		_sign == '+' ? channel->set_topic_chanop_only(true) : channel->set_topic_chanop_only(false);
	}

	void	Mode::_exec_chanMode(User &author) {
		if (_args.size() == 2) {
			_return_channel_modes(author);
			return ;
		}
		vector_string::const_iterator	it_args = _args.begin() + 3;
		Channel	*channel = author.find_channel(_args[1]);
		std::string		mode_string = _args[2];
		for (std::string::const_iterator it = mode_string.begin() ; it != mode_string.end() ; ++it) {
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
					_channel_mode_t(channel, it_args, author);
					break;
				case 'n':
					_channel_mode_n(channel, it_args, author);
					break;
				case 'm':
					_channel_mode_m(channel, it_args, author);
					break;
				case 'v':
					_channel_mode_v(channel, it_args, author);
					break;
				case 'p':
					_channel_mode_p(channel, it_args, author);
					break;
				case 's':
					_channel_mode_s(channel, it_args, author);
					break;
				case 'i':
					_channel_mode_i(channel, it_args, author);
					break;
				// Args needed
				case 'o':
					_channel_mode_o(channel, it_args, author);
					++it_args;
					break;
				case 'l':
					_channel_mode_l(channel, it_args, author);
					++it_args;
					break;
				case 'b':
					_channel_mode_b(it - mode_string.begin(), channel, it_args, author);
					++it_args;
					break;
				case 'k':
					_channel_mode_k(channel, it_args, author);
					++it_args;
					break;
			}
		}
		_build_return_message(channel, author);
	}

}
