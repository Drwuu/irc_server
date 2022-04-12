#include "../../headers/server.hpp"
#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include "../../headers/error/error.hpp"
#include <iostream>
#include <istream>

int main() {
	irc::server server;
	irc::parser parser("/toto tata", server.get_map());

	irc::command *cmd;
	try {
		cmd = parser.get_command();
		std::cout << "CMD is: " << cmd->get_name() << std::endl;
	}
	catch (irc::error &e) {
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}
