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
	/* Getters & Setters */
		private:
		public:
			vector_args const	&get_args() const;
			void				set_args(vector_args const &args);
	/* Functions */
		private:
		protected:
			virtual void		_exec_cmd(command const &cmd) const = 0;
			virtual void		_is_valid_args(vector_args const &args) throw() = 0;
		public:
	};
}
