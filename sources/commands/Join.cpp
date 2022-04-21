#include "../../headers/commands/Join.hpp"
#include <cstdio>
#include <sys/_types/_size_t.h>
#include <vector>

namespace irc {
/* Constructors & Destructors */
	Join::~Join() {};
	Join::Join() {};
/* Operators */
/* Functions public */
	void Join::exec_cmd(command const &cmd, User const &user) const {
		(void)cmd;
		(void)user;
	};

	void Join::is_valid_args(Server const *Server, User const &user) const {
		if (_args.size() < 2)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);

		vector_string chans = _get_instructions(_args[1], ',');
		_erase_chars("#&", chans);
		for (size_t i = 0; i < chans.size(); i++)
			dprintf(2, "_erase_chars | chans[%lu] %s\n", i, chans[i].c_str());

		vector_string keys;
		if (_args.size() >= 3)
			keys = _get_instructions(_args[2], ',');

		vec_chan const serv_chans = Server->get_channel_list();
 		// fixme: is it_args invalid if multiple channels with ',' but only one bad
		for (size_t i = 0; i < chans.size(); i++) {
			vec_cit_chan mchan = Server->find_chan_name(chans[i], serv_chans);
			if (serv_chans.size() == 0 || mchan == serv_chans.end())
				throw error(chans[i] + " :No such channel", ERR_NOSUCHCHANNEL);
			vec_user users = (*mchan)->get_user_list();
			if (users.size() >= (*mchan)->get_userlimit())
				throw error(chans[i] + " :Cannot join channel (+l)", ERR_CHANNELISFULL);
			if ((*mchan)->is_invite())
				throw error(chans[i] + " :Cannot join channel (+i)", ERR_INVITEONLYCHAN);
			vec_user bannedUsers = (*mchan)->get_banned_user();
			for (size_t j = 0; j < bannedUsers.size(); j++) {
				if (user.get_username() == bannedUsers[j]->get_username())
					throw error(chans[i] + " :Cannot join channel (+b)", ERR_BANNEDFROMCHAN);
			}
		}
		for (size_t i = 0; i < keys.size(); i++) {
			if (i >= chans.size())															// ignore too much keys
				break ;
			vec_cit_chan mchan = Server->find_chan_name(chans[i], serv_chans);				// pos of key must be the same as chan
			if (keys[i] != (*mchan)->get_key())
				throw error(chans[i] + " :Cannot join channel (+k)", ERR_BADCHANNELKEY);
		}
	};

/* Functions private */
	/*
	*	*RESUME*
	*	_get_instructions: get instructions inside the string arg, separated by the separator
	*/
	vector_string Join::_get_instructions(string const &arg, char const separator) const {
		vector_string instructions;
		for (size_t pos = 0; pos < arg.size(); pos++) {
			string marg;
			while (arg[pos]) {
				if (arg[pos] == separator)
					break ;
				marg.push_back(arg[pos++]);
			}
			instructions.push_back(marg);
		}
		return instructions;
	}
	/*
	*	*RESUME*
	*	_skip_chars: erase chars found in args
	*/
	void Join::_erase_chars(string const chars, vector_string &args) const {
		for (vec_it_string it_args = args.begin(); it_args != args.end(); it_args++)		// browse args
			for (string::iterator it = (*it_args).begin(); it != (*it_args).end(); it++)	// browse string in args
				for (size_t j = 0; j < chars.size(); j++)									// check if char in string match char in chars
					if (*it == chars[j])
						(*it_args).erase(it);												// erase the char targeted
	}
}
