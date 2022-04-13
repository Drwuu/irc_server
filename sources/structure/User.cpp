#include "../../headers/structure/User.hpp"
#include "../../headers/structure/Server.hpp"
User::User(){}
User::~User(){}
ChanStatus::ChanStatus(Channel * channel):channel(channel),is_admin(false),is_banned(false),is_mute(false),is_operator(false){}

const std::string User::get_username() const{
	return (this->_username);}
const std::string User::get_nickname() const{
	return (this->_nickname);}
const std::string User::get_password() const{
	return (this->_password);}
const std::string User::get_uuid() const{
	return (this->_uuid);}
const std::string User::get_port() const{
	return (this->_port);}
const std::string User::get_ip() const{
	return (this->_ip);}
const std::string User::get_mode() const{
	return (this->_mode);}
bool User::get_registered_status() const{
	if (_is_registered == true)
		return true;
	return false;}
std::vector<std::string> User::get_past_username(){
	return (this->_past_username);}
std::vector<ChanStatus> User::get_chan_list(){
	return (this->_chan_list);}
ChanStatus * User::get_chanstatus_from_list(Channel * channel){
	for (std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end();++it)
	{
		if ((*(it)).channel->get_name() == channel->get_name())
			return &(*it);
	}
	std::cout << "Channel not found\n";
	return nullptr;
}

void User::set_password(std::string password){
	this->_password = password;}
void User::set_ip(std::string ip){
	this->_ip = ip;}
void User::set_port(std::string port){
	this->_port = port;}
void User::set_uuid(std::string uuid){
	this->_uuid = uuid;}
void User::set_username(std::string username){
	if (this->_username.c_str()){
		this->_past_username.push_back(this->_username);
	}
	this->_username = username;}
void User::set_nickname(std::string nickname){
	this->_nickname = nickname;}

void User::join_channel(Server & server,std::string channel){
	if (this->get_registered_status() == false)
		return ; // User not registered : send error message
	for(std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end(); ++it){
		if ((*it).channel->get_name() == channel){
			if ((*it).is_banned == true)
			{
				return ; // User banned from channel : send error message
			}
			return ; // Channel already joined : send error message
		}
	}
	for (std::vector<Channel *>::const_iterator it = server.get_channel_list().begin(); it != server.get_channel_list().end(); ++it){
		if ((*it)->get_name() == channel){
			ChanStatus chan(*it);
			if ((*it)->is_moderated() == true)
				chan.is_mute = true;
			chan.channel->add_user(this);
			get_chan_list().push_back(chan);
			return ; // Channel found
		}
	}
	// Channel not found in channel list : create it
	// WIP need to add mod to the channel creation
	Channel *new_chan = new Channel(channel);
	server.add_channel(*new_chan);
	ChanStatus chan_status(new_chan);
	chan_status.is_operator = true;
	chan_status.is_mute = false;
	get_chan_list().push_back(chan_status);
	new_chan->add_user(this);
}

void User::leave_channel(std::string channel){
	for (std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end(); ++it){
		if ((*it).channel->get_name() == channel){
			(*it).channel->del_user(this);
			get_chan_list().erase(it);
			break;
		}
	}
}

void User::send_message(Server & server, Channel & channel, std::string msg){
	for(std::vector<Channel *>::const_iterator it = server.get_channel_list().begin(); it != server.get_channel_list().end(); ++it){
		if ((*it)->get_name() == channel.get_name()){
			if (channel.is_moderated() == true && get_chanstatus_from_list(&channel)->is_mute == true) // si le channel est restreint et que l'utilisateur est mute
				return; // if the server is muted, the user can't send message
			for(std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
				(*it)->receive_message(server,channel,msg); // send message to all users in the channel
			}
		}
	}
	// Channel not found in channel list : send error
}

void User::send_message(Server & server,User & user, std::string msg){
	if (this->get_registered_status() == false)
		return; // if the user is not registered, he can't send message : return error
	for (std::vector<User *>::const_iterator it = server.get_user_list().begin(); it != server.get_user_list().end(); ++it){
		if ((*it)->get_username() == user.get_username()){
			(*it)->receive_message(server,*this,msg);
			return; // user found in server list : send message
		}
	}
	// user not found in server list : send error
}

void User::kick_user(User & user, Channel & channel,std::string msg){
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (this->get_chanstatus_from_list(&channel)->is_operator == true)
	{
		for (std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
			if ((*it)->get_username() == user.get_username()){
				user.leave_channel(channel.get_name());
				std::cout << msg << std::endl;
				return; // user found in channel list : kick user
			}
			else if (it == channel.get_user_list().end() - 1)
				return; // user not found in channel list : send error
		}
	}
	else
	{
		// User not operator: send error
	}
}

void User::ban_user(User & user, Channel & channel){
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (this->get_chanstatus_from_list(&channel)->is_operator == true)
	{
		for (std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
			if ((*it)->get_username() == user.get_username()){

				if (user.get_chanstatus_from_list(&channel)->is_banned == true)
					return; // user already banned from the channel
				else
					user.get_chanstatus_from_list(&channel)->is_banned = true;
				return; // user found in channel list : ban user but do not kick him
			}
			else if (it == channel.get_user_list().end() - 1)
				return; // user not found in channel list : send error
		}
	}
	else
	{
		// User not operator: send error
	}
}

void User::unban_user(User & user,Channel & channel)
{
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (this->get_chanstatus_from_list(&channel)->is_operator == true)
	{
		for (std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
			if ((*it)->get_username() == user.get_username()){
				if (user.get_chanstatus_from_list(&channel)->is_banned == false)
					return; // user not banned from the channel
				else
					user.get_chanstatus_from_list(&channel)->is_banned = false;
				return; // user found in channel list : ban user but do not kick him
			}
			else if (it == channel.get_user_list().end() - 1)
				return; // user not found in channel list : send error
		}
	}
	else
	{
		// User not operator: send error
	}
}

void User::op_user(User & user, Channel & channel){
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (this->get_chanstatus_from_list(&channel)->is_operator == true)
	{
		for (std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
			if ((*it)->get_username() == user.get_username()){
				if (user.get_chanstatus_from_list(&channel)->is_operator == true)
				{
					user.get_chanstatus_from_list(&channel)->is_operator = false;
					return;
				}
				else
					user.get_chanstatus_from_list(&channel)->is_operator = true;
				return; // user found in channel list : op user
			}
			else if (it == channel.get_user_list().end() - 1)
				return; // user not found in channel list : send error
		}
	}
	else
	{
		// User not operator: send error
	}
}

void User::change_topic(Channel & channel,std::string topic){
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (channel.is_topic() == true){
		if (this->get_chanstatus_from_list(&channel)->is_operator == true){
			channel.set_topic(topic);
		}
		else{
			// User not operator: send error
		}
	}
	// normal user can change topic
	channel.set_topic(topic);
}

void User::unmute_user(User & user, Channel & channel){
	if (this->get_chanstatus_from_list(&channel) == NULL)
		return; // user not found in channel list : send error
	if (this->get_chanstatus_from_list(&channel)->is_operator == true)
	{
		for (std::vector<User *>::const_iterator it = channel.get_user_list().begin(); it != channel.get_user_list().end(); ++it){
			if ((*it)->get_username() == user.get_username()){
				if (user.get_chanstatus_from_list(&channel)->is_mute == true){
					user.get_chanstatus_from_list(&channel)->is_mute = false;
					return; // user was mute
				}
				else
					user.get_chanstatus_from_list(&channel)->is_mute = false;
				return; // user was unmute
			}
			else if (it == channel.get_user_list().end() - 1)
				return; // user not found in channel list : send error
		}
	}
	else
	{
		// User not operator: send error
	}
}
