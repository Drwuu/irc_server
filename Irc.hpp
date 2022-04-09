#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <vector>

typedef struct s_chanstatus
{
	bool is_admin;
	bool is_operator;
	Channel *channel;}				t_chanstatus;


#endif
