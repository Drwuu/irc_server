#include "User.hpp"

User::User(){}
User::~User(){}

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
std::vector<std::string> User::get_past_username(){
	return (this->_past_username);}
std::vector<ChanStatus> User::get_chan_list(){
	return (this->_chan_list);}

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
	for(std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end(); ++it){
		if ((*it).channel->get_name() == channel){
			return ;
		}
	}
	for (std::vector<Channel *>::const_iterator it = server.get_channel_list().begin(); it != server.get_channel_list().end(); ++it){
		if ((*it)->get_name() == channel){
			ChanStatus chan(*it,false,false);
			get_chan_list().push_back(chan);
			return ;
		}
	}
	Channel *new_chan = new Channel(channel);
	server.add_channel(*new_chan);
	ChanStatus chan_status(new_chan,true,true);
	get_chan_list().push_back(chan_status);
	new_chan->add_user(this);
}

void User::leave_channel(const Server & server, std::string channel){
	for (std::vector<ChanStatus>::iterator it = get_chan_list().begin(); it != get_chan_list().end(); ++it){
		if ((*it).channel->get_name() == channel){
			(*it).channel->del_user(this);
			get_chan_list().erase(it);
			break;
		}
	}
}
