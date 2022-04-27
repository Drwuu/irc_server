
#include "../../headers/proxy/Address_ipv4.hpp"

// Unaccessible
Address_ipv4::Address_ipv4() { }
Address_ipv4::~Address_ipv4() { }

// struct sockaddr_in {
	// __uint8_t       sin_len;
	// sa_family_t     sin_family;
	// in_port_t       sin_port;
	// struct  in_addr sin_addr;
	// char            sin_zero[8];
// };

Address_ipv4::Address_ipv4(const port_type &port)
	: _address(), _family(AF_INET), _len(0), _port(port) {
	_address.sin_family = _family;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_len = sizeof(_address);
	_len = sizeof(_address);
}

Address_ipv4::Address_ipv4(const Address_ipv4 &source) { *this = source; }

Address_ipv4	&Address_ipv4::operator=(const Address_ipv4 &source) {
	_address = source._address;
	_family = source._family;
	_len = source._len;
	_port = source._port;
	return *this;
}

Address_ipv4::value_type		&Address_ipv4::operator*() { return _address; }
Address_ipv4::value_type		Address_ipv4::data() { return _address; }

void							Address_ipv4::set_len(const len_type &len) { _len = len; }
void							Address_ipv4::set_port(const port_type &port) { _port = port; }
void							Address_ipv4::set_family(const family_type &family) { _family = family; }

Address_ipv4::port_type			Address_ipv4::get_port() const { return _port; }
Address_ipv4::address_type		Address_ipv4::get_address() const { return _address.sin_addr.s_addr; }
std::string						Address_ipv4::get_readable_address() const {
	int		address_length = INET_ADDRSTRLEN;
	char	tmp[address_length];
	bzero(tmp, address_length);
	inet_ntop(_family, &_address.sin_addr, tmp, (socklen_t)address_length);
	return std::string(tmp, address_length);
}

Address_ipv4::len_type			Address_ipv4::get_len() const { return _len; }
Address_ipv4::family_type		Address_ipv4::get_family() const { return _family; }

void			Address_ipv4::renew(const port_type &port) {
	bzero(&_address, sizeof(value_type));
	*this = Address_ipv4(port);
}
