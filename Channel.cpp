#include "Channel.hpp"

const std::string& Channel::get_name() const{
	return (this->_name);}
const std::string& Channel::get_prefix() const{
	return (this->_prefix);}

const std::string& Channel::get_mask() const{
	return (this->_mask);}

const std::string& Channel::get_mode() const{
	return (this->_mode);}

const std::string& Channel::get_flag() const{
	return (this->_flag);}

const int & Channel::get_lifetime() const{
	return (this->_lifetime);}

const int & Channel::get_opdelay() const{
	return (this->_opdelay);}

const int & Channel::get_userlimit() const{
	return (this->_userlimit);}

void Channel::set_name(const std::string & name){
	this->_name = name;}

void Channel::set_prefix(const std::string & prefix){
	this->_prefix = prefix;}

void Channel::set_mask(const std::string & mask){
	this->_mask = mask;}

void Channel::set_mode(const std::string & mode){
	this->_mode = mode;}

void Channel::set_flag(const std::string & flag){
	this->_flag = flag;}

void Channel::set_lifetime(const int & lifetime){
	this->_lifetime = lifetime;}

void Channel::set_opdelay(const int & opdelay){
	this->_opdelay = opdelay;}

void Channel::set_userlimit(const int & userlimit){
	this->_userlimit = userlimit;}

Channel::Channel(){}

Channel::Channel(Channel const & copy){*this = copy;}

Channel & Channel::operator=(Channel const & op){}

Channel::~Channel(){}
