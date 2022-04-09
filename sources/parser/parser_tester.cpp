#include "../../headers/parser/parser.hpp"
#include <iostream>
#include <istream>

int main() {
	std::string line = "/aDmin tata";
	irc::parser parser(line);

	std::string cmd;
	cmd = parser.get_command();
	std::cout << "My cmd is : " << cmd << std::endl;
	return 0;
}
