#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"
#include "User.hpp"

class Channel{
	private:
		std::string	_name;
		std::string _topic;
		std::string	_prefix;
		std::string	_mask;
		std::string	_mode;
		std::string	_mask;
		std::string	_flag;
		int			_lifetime;
		int			_opdelay;
		int			_userlimit;
		std::vector<User *>	_user_list;
		std::vector<User *> _ban_list; // Needed for ISBANNED function
		Channel & operator=(Channel const & op);
		Channel(Channel const & copy);
	protected:
		/*Arg*/
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		const std::string&			get_name() const;
		const std::string&			get_prefix() const;
		const std::string&			get_mask() const;
		const std::string&			get_mode() const;
		const std::string&			get_flag() const;
		const std::string&			get_topic() const;
		const int&					get_lifetime() const;
		const int&					get_opdelay() const;
		const int&					get_userlimit() const;
		const std::vector<User *>	get_user_list() const;
		const std::vector<User *>	get_banned_user() const; // Needed for ISBANNED function

		void						set_name(const std::string& name);
		void						set_prefix(const std::string& prefix);
		void						set_mask(const std::string& mask);
		void						set_mode(const std::string& mode);
		void						set_flag(const std::string& flag);
		void						set_topic(const std::string& topic); // Use for TOPIC function
		void						set_lifetime(const int& lifetime);
		void						set_opdelay(const int& opdelay);
		void						set_userlimit(const int& userlimit);

		void						add_user(User * user);
		void						del_user(User * user);
		void						ban_user(std::string nick);
		void						unban_user(std::string nick);
};


#endif
