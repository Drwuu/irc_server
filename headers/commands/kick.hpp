#pragma once
#include "command.hpp"

namespace irc {
	class kick: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
			kick();
			kick(kick const &src);
		public:
			kick(vector_args const &args);
			virtual ~kick();
	/* Operators */
		private:
			kick &operator=(kick const &src);
		public:
	/* Functions */
		private:
		public:
	};
}