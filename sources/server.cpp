#include "../headers/server.hpp"
#include "../headers/commands/invite.hpp"
#include "../headers/commands/kick.hpp"
#include "../headers/commands/mode.hpp"

namespace irc {
/* Constructors & Destructors */
	server::~server() {
		for (map_iterator_cmd it = _map.begin(); it != _map.end(); it++)
			if (it->second)
				delete it->second;
		_map.clear();
	};
	server::server(): _map() {
		_map.insert(std::make_pair("INVITE", new invite(vector_args(1, "INVITE"))));
		_map.insert(std::make_pair("KICK", new kick(vector_args(1, "KICK"))));
		_map.insert(std::make_pair("MODE", new mode(vector_args(1, "MODE"))));
	};
	server::server(server const &src) {
		*this = src;
	};

/* Operators */
	server &server::operator=(server const &src) {
		_map = src._map;
		return *this;
	};
/* Getters & Setters */
	map_cmd &server::get_map() {
		return _map;
	};

/* Functions */
}
