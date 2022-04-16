#pragma once
#include "../irc.hpp"
#include "../structure/server.hpp"

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
		public:
			virtual void		is_valid_args(server const *server) const = 0;
			virtual void		exec_cmd(command const &cmd) const = 0;
	};
}
