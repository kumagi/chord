#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdint.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <msgpack.hpp>

#include <boost/unordered_map.hpp>
#include <netdb.h>
#include <stdexcept>
#include "util.h"

class address {
public:
	address():ip(),port(){}
	address(uint32_t i, uint16_t p):ip(i),port(p){}
	address(const address& a):ip(a.ip),port(a.port){}
	const address& operator= (const address& o);
	bool operator== (const address& o) const{return ip == o.ip && port == o.port;}
	bool operator!= (const address& o) const{return !operator==(o);}
	bool operator<  (const address& o) const{
		if(ip < o.ip) return true;
		if(o.ip < ip) return false;
		if(port < o.port) return true;
		return false;
	}

	friend std::ostream& operator<< (std::ostream& stream, const address& a);

	MSGPACK_DEFINE(ip,port);
public:
	uint32_t ip;
	uint16_t port;
};

#endif
