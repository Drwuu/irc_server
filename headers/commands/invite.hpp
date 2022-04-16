#pragma once
#include "command.hpp"

namespace irc {
	class server;
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
			void	exec_cmd(command const &cmd) const;
			void	is_valid_args(server const *server) const;
	};
}
