#ifndef ACCEPTER_H
#define ACCEPTER_H
#include "address.h"
#include "io.h"
#include "event.h"
#include "io.h"
#include "buffered_connection.h"

class acceptor : io_base{
	typedef io<acceptor> io_t;
public:
	acceptor(const address& add, loop* lo){
		fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(add.port);
		addr.sin_addr.s_addr = add.ip;
		bind(fd_, (sockaddr *)&addr, sizeof(addr));
		listen(fd_,2047);

		sock_ = new io_t(this, lo);
	}
	
	int fd()const{return fd_;}
	bool on_readable(){
		const int new_fd = ::accept(fd(), NULL, NULL);
		buffered_connection* new_conn = new buffered_connection(new_fd);
		io<buffered_connection> new_socket(new_conn, sock_->get_loop());
		new_socket.attach(io_type::READ);
		return true;
	}
	bool on_writable(){
		assert(!"it must not be called");
	}
	virtual ~acceptor(){
		delete sock_;
	}
private:
	int fd_;
	io_t* sock_;
	
	acceptor(const acceptor&);
	acceptor& operator=(const acceptor&);
	acceptor();
	static void set_nonblock(int fd){
		int flags;
		if ((flags = fcntl(fd, F_GETFL, 0)) < 0 
				|| fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
			assert(!"fcntl(2)");
		}
	}
};

#endif
