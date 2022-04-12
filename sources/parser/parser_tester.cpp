#include "../../headers/server.hpp"
#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include "../../headers/error/error.hpp"
#include <iostream>
#include <istream>

int main() {
	irc::server server;
	irc::parser parser("/invite tata", server.get_map());
	irc::vector_args cmd;
	try {
		cmd = parser.get_command_infos();
		std::cout << "CMD is: " << *cmd.begin() << std::endl;
		std::cout << "ARG1 is: " << *(cmd.begin()+1) << std::endl;
		// std::cout << "ARG2 is: " << *(cmd.begin()+2) << std::endl;
	}
	catch (irc::error &e) {
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}
