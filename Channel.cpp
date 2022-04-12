#include "Channel.hpp"

const std::string& Channel::get_name() const{
	return (this->_name);}
const std::string& Channel::get_prefix() const{
	return (this->_prefix);}

const int & Channel::get_userlimit() const{
	return (this->_userlimit);}

const std::vector<User *> Channel::get_user_list() const{
	return (this->_user_list);}

const std::vector<User *> Channel::get_banned_user() const{
	return (this->_ban_list);}

const bool Channel::is_private() const{
	return (this->_is_private);}
const bool Channel::is_secret() const{
	return (this->_is_secret);}
const bool Channel::is_invite() const{
	return (this->_is_invite);}
const bool Channel::is_topic() const{
	return (this->_is_topic);}
const bool Channel::is_no_external_msg() const{
	return (this->_is_no_external_msg);}
const bool Channel::is_moderated() const{
	return (this->_is_moderated);}
const bool Channel::is_limited() const{
	return (this->_is_limited);}

void Channel::set_name(const std::string & name){
	this->_name = name;}

void Channel::set_prefix(const std::string & prefix){
	this->_prefix = prefix;}

void Channel::set_lifetime(const int & lifetime){
	this->_lifetime = lifetime;}

void Channel::set_opdelay(const int & opdelay){
	this->_opdelay = opdelay;}

void Channel::set_userlimit(const int & userlimit){
	this->_userlimit = userlimit;}

Channel::Channel(){}

Channel::Channel(std::string name): _name(name){}

Channel::Channel(Channel const & copy){*this = copy;}

Channel & Channel::operator=(Channel const & op){}

Channel::~Channel(){}
