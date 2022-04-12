#include "Irc.hpp"
#include "Channel.hpp"

ChanStatus::ChanStatus(Channel *channel, bool is_admin,bool is_operator):channel(channel),is_admin(is_admin),is_operator(is_operator){}

ChanStatus::ChanStatus(Channel * channel):channel(channel),is_admin(false),is_banned(false),is_mute(false),is_operator(false){}
