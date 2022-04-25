/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaman <mhaman@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 16:58:42 by guhernan          #+#    #+#             */
/*   Updated: 2022/04/23 19:45:01 by guhernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/proxy/Address.hpp"

// Unaccessible
Address_ipv6::Address_ipv6() { }
Address_ipv6::~Address_ipv6() { }

Address_ipv6::Address_ipv6(const port_type &port)
	: _address(), _family(AF_INET6), _len(0), _port(port) {
	_address.sin6_family = _family;
	_address.sin6_port = htons(_port);
	_address.sin6_addr = in6addr_any;
	_address.sin6_len = sizeof(_address);

	_len = sizeof(_address);
}

Address_ipv6::Address_ipv6(const Address_ipv6 &source) { *this = source; }

Address_ipv6	&Address_ipv6::operator=(const Address_ipv6 &source) {
	_address = source._address;
	_family = source._family;
	_len = source._len;
	_port = source._port;
	return *this;
}

Address_ipv6::value_type		&Address_ipv6::operator*() { return _address; }
Address_ipv6::value_type		Address_ipv6::data() { return _address; }

void							Address_ipv6::set_len(const len_type &len) { _len = len; }
void							Address_ipv6::set_port(const port_type &port) { _port = port; }
void							Address_ipv6::set_family(const family_type &family) { _family = family; }

Address_ipv6::port_type			Address_ipv6::get_port() const { return _port; }
Address_ipv6::address_type		Address_ipv6::get_address() const { return _address.sin6_addr; }
std::string						Address_ipv6::get_readable_address() const {
	int		address_length = INET6_ADDRSTRLEN;
	char	tmp[address_length];
	bzero(tmp, address_length);
	inet_ntop(_family, &_address.sin6_addr, tmp, (socklen_t)address_length);
	return std::string(tmp, address_length);
}

Address_ipv6::len_type			Address_ipv6::get_len() const { return _len; }
Address_ipv6::family_type		Address_ipv6::get_family() const { return _family; }

void			Address_ipv6::renew(const port_type &port) {
	bzero(&_address, sizeof(value_type));
	*this = Address_ipv6(port);
}
