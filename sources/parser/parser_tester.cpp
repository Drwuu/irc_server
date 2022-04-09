#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include <iostream>
#include <istream>

int main() {
	std::string line = "/admin tata";
	irc::parser parser(line);

	irc::command *cmd;
	std::string arg = parser.get_first_arg();
	cmd = parser.get_command(arg);
	if (cmd)
		std::cout << "My cmd is : " << cmd->get_name() << std::endl;
	else
		std::cout << "Error(" + std::string(ERR_UNKNOWNCOMMAND) + "): " + arg + " Unknown command" << std::endl;
	return 0;
}
