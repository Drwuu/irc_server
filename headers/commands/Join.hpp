#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Join: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			Join(Join const &src);
		public:
			Join();
			virtual ~Join();
	/* Operators */
		private:
			Join &operator=(Join const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(command const &cmd, User const &user) const;
			void	is_valid_args(Server const *Server, User const &user) const;
	};
}
