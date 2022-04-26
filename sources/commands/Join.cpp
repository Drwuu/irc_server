#include "../../headers/commands/Join.hpp"
#include <cstdio>
#include <sstream>
#include <sys/_types/_size_t.h>
#include <vector>

namespace irc {
/* Constructors & Destructors */
	Join::~Join() {};
	Join::Join() {};
	Join::Join(Server *server): command(server), _chans(), _keys() {};
/* Operators */
/* Functions */
	void Join::exec_cmd(User &user) {
		vec_chan const serv_chans = _server->get_channel_list();
		for (size_t i = 0; i < _chans.size(); i++) {
			vec_cit_chan mchan = _server->find_chan_name(_chans[i], serv_chans);
			if (mchan != serv_chans.end()) {												// if it founds a channel, add user on it
				ChanStatus status(*mchan);
				user.join_channel(status);													// create channel and add user on it
				(*mchan)->add_user(&user);
				(*mchan)->transmit_message( " JOIN " + (*mchan)->get_name(), &user);

				std::stringstream s;
				s << ":" << user.get_nickname() << " JOIN " << (*mchan)->get_name() << "\r\n\0";
				Proxy_queue::Write *msg = new Proxy_queue::Write(user.get_socket()->get_fd(), s.str().c_str());
				_server->get_event_list().push_back(msg);

				// string str = _send_RPL(RPL_NOTOPIC, user, **mchan, " :No topic is set\r\n\0");
				// Proxy_queue::Write *msg = new Proxy_queue::Write(user.get_socket()->get_fd(), str.c_str());
				// _server->get_event_list().push_back(msg);

				string str = _send_RPL_user_list(user, **mchan);
				Socket<Address_ipv6> const *sock = user.get_socket();
				msg = new Proxy_queue::Write(sock->get_fd(), str.c_str());
				_server->get_event_list().push_back(msg);

				str = _send_RPL(RPL_ENDOFNAMES, user, **mchan, " :End of /NAMES list.\r\n\0");
				msg = new Proxy_queue::Write(user.get_socket()->get_fd(), str.c_str());
				_server->get_event_list().push_back(msg);
			}
			else {
				Channel *chan = new Channel(_chans[i]);
				ChanStatus status(chan);
				chan->add_user(&user);
				user.join_channel(status);													// create channel and add user on it
				chan->add_operator(&user);
				user.set_chan_status(chan, true);
				_server->add_channel(chan);

				std::stringstream s;
				s << ":" << user.get_nickname() << " JOIN " << chan->get_name() << "\r\n\0";
				Proxy_queue::Write *msg = new Proxy_queue::Write(user.get_socket()->get_fd(), s.str().c_str());
				_server->get_event_list().push_back(msg);

				// string str = _send_RPL(RPL_NOTOPIC, user, *chan, " :No topic is set\r\n\0");
				// Proxy_queue::Write *msg = new Proxy_queue::Write(user.get_socket()->get_fd(), str.c_str());
				// _server->get_event_list().push_back(msg);

				string str = _send_RPL_user_list(user, *chan);
				msg = new Proxy_queue::Write(user.get_socket()->get_fd(), str.c_str());
				_server->get_event_list().push_back(msg);

				str = _send_RPL(RPL_ENDOFNAMES, user, *chan, " :End of /NAMES list.\r\n\0");
				msg = new Proxy_queue::Write(user.get_socket()->get_fd(), str.c_str());
				_server->get_event_list().push_back(msg);
			}
		}
	};

	bool Join::is_valid_args(User const &user) {
		if (_args.size() < 2)
			throw error(_args[0] + " :Not enough parameters", ERR_NEEDMOREPARAMS);

		_chans = _get_instructions(_args[1], ',');

		if (_args.size() >= 3)																// get keys
			_keys = _get_instructions(_args[2], ',');

		vec_chan const serv_chans = _server->get_channel_list();
		if (serv_chans.size() == 0)															// if no chans, all chans will be created
			return true;
 		// fixme: is it_args invalid if multiple channels with ',' but only one bad
		for (size_t i = 0; i < _chans.size(); i++) {
			vec_cit_chan mchan = _server->find_chan_name(_chans[i], serv_chans);
			if (mchan != serv_chans.end()) {												// if it founds a channel
				vec_user users = (*mchan)->get_user_list();
				if (users.size() >= (*mchan)->get_userlimit())
					throw error(_chans[i] + " :Cannot join channel (+l)", ERR_CHANNELISFULL);
				if ((*mchan)->is_invite()) {
					if ((*mchan)->is_invite(user.get_nickname()) == false)
						throw error(_chans[i] + " :Cannot join channel (+i)", ERR_INVITEONLYCHAN);
				}
				vector_string bannedUsers = (*mchan)->get_banned_user();
				for (size_t j = 0; j < bannedUsers.size(); j++) {
					if (user.get_nickname() == bannedUsers[j])
						throw error(_chans[i] + " :Cannot join channel (+b)", ERR_BANNEDFROMCHAN);
				}
			}
		}
		for (size_t i = 0; i < _keys.size(); i++) {
			if (i >= _chans.size())															// ignore too much _keys
				break ;
			vec_cit_chan mchan = _server->find_chan_name(_chans[i], serv_chans);			// pos of key must be the same as chan
			if (mchan != serv_chans.end()) {												// if it founds a channel
				if (_keys[i] != (*mchan)->get_key())										// check key
					throw error(_chans[i] + " :Cannot join channel (+k)", ERR_BADCHANNELKEY);
			}
		}
		return true;
	};

/* Functions private */
	string Join::_send_RPL_user_list(User const &user, Channel const &chan) const {
		std::stringstream s;
		s << ":" << _server->get_name() << " " << RPL_NAMREPLY << " "
		<< user.get_nickname() << " @ " << chan.get_name() << " :";

		vec_user chanUsers = chan.get_user_list();
		for (std::vector<User *>::reverse_iterator it = chanUsers.rbegin(); it != chanUsers.rend(); ++it) {
			std::vector<ChanStatus> chans = (*it)->get_chan_list();
			std::vector<ChanStatus>::const_iterator userStatut = (*it)->get_chanstatus_from_list(chan, chans);
			if (userStatut != chans.end() && userStatut->is_operator)
				s << "@";
			else if (userStatut != chans.end() && !userStatut->is_mute && userStatut->channel->is_moderated())
				s << "+";
			s << (*it)->get_nickname() << " ";
		}
		s << "\r\n\0";
		return s.str();
	}
	string Join::_send_RPL(rpl_type const &rpl, User const &user, Channel const &chan, string const &msg) const {
		std::stringstream s;
		s << ":" << _server->get_name() << " " << rpl << " "
		<< user.get_nickname() << " " << chan.get_name() << msg;
		return s.str();
	};
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
