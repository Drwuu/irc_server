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
			vector_args	_args;
		public:
	/* Constructors & Destructors */
		private:
			command(command const &src);
		public:
			command();
			command(vector_args const &args);
			virtual ~command();
	/* Operators */
		private:
			command& operator=(command const &src);
		public:
			string operator()(command const &cmd) const;
	/* Getters & Setters */
		private:
		public:
			vector_args const	&get_args() const;
			void				set_args(vector_args const &args);
	/* Functions */
		private:
		protected:
			string send_instructions(command const &cmd);
		public:
	};
}
