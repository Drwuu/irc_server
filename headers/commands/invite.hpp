#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class invite: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			invite(invite const &src);
		public:
			invite();
			invite(Server *server);
			virtual ~invite();
	/* Operators */
		private:
			invite &operator=(invite const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(User &user);
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
