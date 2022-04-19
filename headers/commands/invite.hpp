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
			virtual ~invite();
	/* Operators */
		private:
			invite &operator=(invite const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(command const &cmd,  User const &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
