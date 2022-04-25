#pragma once
#include "command.hpp"

namespace irc {
	class Server;
	class Part: public command {
	/* Typedefs */
		private:
		public:
	/* Variables */
		private:
			std::vector<std::string>	_chan_list;
		public:
	/* Constructors & Destructors */
		private:
			Part(Part const &src);
		public:
			Part();
			Part(Server *server);
			virtual ~Part();
	/* Operators */
		private:
			Part &operator=(Part const &src);
		public:
	/* Functions */
		private:
			bool is_valid_channel(const std::string &channel) const;
		public:
			void			exec_cmd(User &user);
			virtual bool	is_valid_args(User const &user);
	};
}
