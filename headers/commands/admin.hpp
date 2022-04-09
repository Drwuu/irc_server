#pragma once
#include "command.hpp"

namespace irc {
	class admin: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			admin();
		public:
			admin(string const &name);
			admin(admin const &src);
			virtual ~admin();
	/* Operators */
		private:
		public:
			admin &operator=(admin const &src);
	/* Functions */
		private:
		public:
	};
}
