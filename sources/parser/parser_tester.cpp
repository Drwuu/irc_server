#include "../../headers/server.hpp"
#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include "../../headers/error/error.hpp"
#include <iostream>
#include <istream>

int main() {
	irc::server server;
	irc::parser parser("/invite tata", server.get_map());
	try {
		parser.fill_command();
		irc::command *cmd = server.get_map().find("INVITE")->second;
		std::cout << "CMD is: " << cmd->get_args()[0] << std::endl;
		std::cout << "ARG1 is: " << cmd->get_args()[1] << std::endl;
		// std::cout << "ARG2 is: " << cmd->get_args()[2] << std::endl;
	}
	catch (irc::error &e) {
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}
