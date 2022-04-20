#include "../../headers/commands/Nick.hpp"

namespace irc {
/* Constructors & Destructors */
	Privmsg::~Privmsg() {};
	Privmsg::Privmsg(){
		_args.push_back("PRIVMSG");
	};
}
