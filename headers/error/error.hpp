#pragma once
#include "../irc.hpp"
#include <exception>
#include <sstream>

namespace irc {
	class error: public std::exception {
	/* Typedefs */
		private:
		public:
	/* Enum */
		private:
		public:
	/* Variables */
		private:
			std::string	_msg;
		public:

	/* Constructors & Destructors */
		private:
			error();
			// error(error const &src);
		public:
			error(string const &arg, error_type const &type);
			virtual ~error() throw();
	/* Operators */
		private:
			error	&operator=(error const &src);
		public:
	/* Getters & Setters */
		private:
		public:
	/* Functions */
		private:
		public:
			virtual const char	*what() const throw();
	};
}
