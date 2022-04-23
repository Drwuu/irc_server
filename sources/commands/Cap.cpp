#include "../../headers/commands/Cap.hpp"


irc::Cap::Cap(const Cap &) { }

irc::Cap::Cap() { }
irc::Cap::Cap(Server *) { }
irc::Cap::~Cap() { }

bool	irc::Cap::is_valid_args(User const &) { return true; }

void	irc::Cap::exec_cmd(User &) {}
