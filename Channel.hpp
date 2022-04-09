#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

class Channel{
	private:
		std::string	_name;
		std::string	_prefix;
		std::string	_mask;
		std::string	_mode;
		std::string	_mask;
		std::string	_flag;
		int			_lifetime;
		int			_opdelay;
		int			_userlimit;
		Channel & operator=(Channel const & op);
		Channel(Channel const & copy);
	protected:
		/*Arg*/
	public:
		Channel();
		~Channel();
		const std::string&	get_name() const;
		const std::string&	get_prefix() const;
		const std::string&	get_mask() const;
		const std::string&	get_mode() const;
		const std::string&	get_flag() const;
		const int&			get_lifetime() const;
		const int&			get_opdelay() const;
		const int&			get_userlimit() const;

		void				set_name(const std::string& name);
		void				set_prefix(const std::string& prefix);
		void				set_mask(const std::string& mask);
		void				set_mode(const std::string& mode);
		void				set_flag(const std::string& flag);
		void				set_lifetime(const int& lifetime);
		void				set_opdelay(const int& opdelay);
		void				set_userlimit(const int& userlimit);
};


#endif
