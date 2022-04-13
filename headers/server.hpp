#pragma once
#include "irc.hpp"
namespace irc {
	class server {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			map_cmd	_map;
		public:

	/* Constructors & Destructors */
		private:
		public:
			server();
			server(server const &src);
			virtual ~server();
	/* Operators */
		private:
		public:
			server	&operator=(server const &src);
	/* Getters & Setters */
		private:
		public:
			map_cmd	&get_map();
	/* Functions */
		private:
		public:
	};
}
