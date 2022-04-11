#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <vector>

struct ChanStatus
{
	ChanStatus(Channel *channel, bool is_admin,bool is_operator);
	bool is_admin;
	bool is_operator;
	bool is_banned;
	Channel *channel;
};


#endif
