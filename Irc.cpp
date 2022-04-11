#include "Irc.hpp"
#include "Channel.hpp"

ChanStatus::ChanStatus(Channel *channel, bool is_admin,bool is_operator):channel(channel),is_admin(is_admin),is_operator(is_operator){}
