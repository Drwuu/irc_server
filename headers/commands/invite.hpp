#pragma once
#include "command.hpp"

namespace irc {
	class invite: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			invite();
			invite(invite const &src);
		public:
			invite(vector_args const &args);
			virtual ~invite();
	/* Operators */
		private:
			invite &operator=(invite const &src);
		public:
	/* Functions */
		private:
			void	_is_valid_args(vector_args const &args) throw();
		public:
			void	exec_cmd(command const &cmd) const;
	};
}
