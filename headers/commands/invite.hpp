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
			invite(string const &name);
			virtual ~invite();
	/* Operators */
		private:
			invite &operator=(invite const &src);
		public:
	/* Functions */
		private:
		public:
	};
}
