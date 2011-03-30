#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <assert.h>

#include <fcntl.h>

#include "address.h"
#include "io.h"

class connection{
public:

	connection(const address& target)
	{
		fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
		set_nonblock(fd_);
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(target.port);
		addr.sin_addr.s_addr = INADDR_ANY;
		connect(fd_, (sockaddr *)&addr, sizeof(addr));
		
	}
	connection(const int fd):fd_(fd){
		set_nonblock(fd);
	}
	int fd()const{return fd_;}
	
	virtual int read(void *buff, int size)const{
		assert(buff);
		assert(0 <= size);
		return ::read(fd_,buff,size);
	}
	virtual int write(const void* buff, int size)const{
		assert(buff);
		assert(0 <= size);
		return ::write(fd_,buff,size);
	}
	virtual ~connection(){
		this->close();
	}
	void close(){
		::close(fd_);
	}
	bool operator==(const connection& rhs)const{
		return fd_ == rhs.fd_;
	}
	bool operator<(const connection& rhs)const{
		return fd_ < rhs.fd_;
	}
private:
	int fd_;
	connection(const connection&);
	connection& operator=(const connection&);
	static void set_nonblock(int fd){
		int flags;
		if ((flags = fcntl(fd, F_GETFL, 0)) < 0 
				|| fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
			assert(!"fcntl(2)");
		}
	}
};


#endif
