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
			mode();
			mode(mode const &src);
		public:
			mode(vector_args const &args);
			virtual ~mode();
	/* Operators */
		private:
			mode &operator=(mode const &src);
		public:
	/* Functions */
		private:
		public:
	};
}