#pragma once
#include "command.hpp"

namespace irc {
	class mode: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			mode(mode const &src);
		public:
			mode();
			virtual ~mode();
	/* Operators */
		private:
			mode &operator=(mode const &src);
		public:
	/* Functions */
		private:
		public:
			void	exec_cmd(command const &cmd) const;
			void	is_valid_args() const;
	};
}
