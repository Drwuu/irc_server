#pragma once
#include "Icommand.hpp"

namespace irc {
	class Admin: public Icommand {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
		public:
			virtual ~Admin() {};
	/* Operators */
		private:
		public:
	/* Functions */
		private:
		public:
			string send_message() const;
	};
}
