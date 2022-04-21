
#include "../../headers/commands/Cap.hpp"


irc::Cap::Cap(const Cap &) { }

irc::Cap::Cap() { }
irc::Cap::Cap(Server *) { }
irc::Cap::~Cap() { }

void	irc::Cap::is_valid_args(Server const *, User const &) const { }

void	irc::Cap::exec_cmd(User &) { }
