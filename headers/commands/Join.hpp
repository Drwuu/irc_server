#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Join: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			vector_string	_chans;
			vector_string	_keys;
		public:
	/* Constructors & Destructors */
		private:
			Join(Join const &src);
		public:
			Join();
			Join(Server *server);
			virtual ~Join();
	/* Operators */
		private:
			Join &operator=(Join const &src);
		public:
	/* Getters */
		private:
		public:
	/* Setters */
		private:
		public:
	/* Functions */
		private:
			bool				_is_valid_channel(const std::string &channel);
			string				_send_RPL_user_list(User const &user, Channel const &chan) const;
			string				_send_RPL(rpl_type const &rpl, User const &user, Channel const &chan, string const &msg) const;
			vector_string		_get_instructions(string const &arg, char const separator) const;
			void				_erase_chars(string const chars, vector_string &vector) const;
		public:
			void				exec_cmd(User &user);
			bool				is_valid_args(User const &user);
	};
}
