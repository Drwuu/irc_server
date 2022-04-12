#pragma once
#include "../irc.hpp"

namespace irc {
	class command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
		protected:
			string	_name;
		public:
	/* Constructors & Destructors */
		private:
		public:
			virtual ~command();
			command();
			command(string const &name);
			command(command const &src);
	/* Operators */
		private:
			command& operator=(command const &src);
		public:
			string operator()(command const &cmd) const;
	/* Getters & Setters */
		private:
		public:
			string const	&get_name() const;
	/* Functions */
		private:
		protected:
			string send_instructions(command const &cmd);
		public:
	};
}
