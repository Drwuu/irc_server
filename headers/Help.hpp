#pragma once
#include "../headers/Help.hpp"
#include "structure/User.hpp"
#include <string>

namespace irc {
	class Server;
	class Help: public User {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Help(Help const &src);
		public:
			Help();
			Help(Server *server);
			virtual ~Help();
	/* Operators */
		private:
			Help &operator=(Help const &src);
		public:
	/* Functions */
		private:
		public:
			virtual void	receive_message(User * user,std::string msg);
			virtual void	send_message(std::string msg, User & user);
	};
}
