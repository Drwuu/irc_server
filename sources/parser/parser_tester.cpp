#include "../../headers/parser/parser.hpp"
#include "../../headers/commands/command.hpp"
#include <iostream>
#include <istream>

int main() {
	std::string line = "/aDmin tata";
	irc::parser parser(line);

	irc::command *cmd;
	cmd = parser.get_command();
	if (cmd)
		std::cout << "My cmd is : " << cmd->get_name() << std::endl;
	else
		std::cout << "Error(" + std::string(ERR_UNKNOWNCOMMAND) + "): " + "cmd" + " Unknown command" << std::endl;
	return 0;
}
