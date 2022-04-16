#include "../../headers/structure/Server.hpp"
#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include "../../headers/error/error.hpp"

int main() {
	Server server;
	irc::parser parser("/invite tata", server.get_map());
	try {
		irc::command const *cmd = parser.get_command();
		(void)cmd;
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
