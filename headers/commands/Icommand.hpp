#pragma once
#include <string>

namespace irc {
	typedef std::string	string;
	class Icommand {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		public:
	/* Constructors & Destructors */
		private:
		public:
			virtual ~Icommand() {};
	/* Operators */
		private:
		public:
	/* Functions */
		private:
		protected:
			virtual string send_message() const = 0;
	};
}
