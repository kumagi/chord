#ifndef WATCHER_H
#define WATCHER_H
#include <ev.h>
#include <assert.h>
#include "loop.h"

namespace io_detail{
template <typename T>
void callback(struct ev_loop* loop, ev_io* w, int revents)
{
  assert(w != NULL);
  T* const sock_data = reinterpret_cast<T*>(w->data);
	bool shutdown = 0;
	if(revents && EV_READ){
		shutdown = sock_data->on_readable() == false ? true : shutdown;
	}
	if(revents && EV_WRITE){
		shutdown = sock_data->on_writable() == false ? true : shutdown;
	}
	if(shutdown){
		ev_io_stop(loop, w);
	}
}
}

namespace io_type{
enum type{
	READ = EV_READ,
	WRITE = EV_WRITE,
	BOTH = EV_READ | EV_WRITE,
};
}

class io_base{ // for private inherit only
public:	
	virtual int fd()const = 0;
	virtual bool on_readable() = 0;
	virtual bool on_writable() = 0;
	virtual ~io_base()= 0;
};
inline io_base::~io_base(){}

template<typename T>
struct io{
	typedef io<T> io_t;
	ev_io io_;
	T* data_; // <- it should inherit io_base
	
	io(T* const t, loop* const lo):data_(t),lo_(lo){}
	void attach(enum io_type::type t){
		ev_io_init(&io_, io_detail::callback<T>, data_->fd(), t);
		ev_io_start(lo_->get_loop(), &io_);
	}
	~io(){assert(data_);delete data_;}
	bool operator<(const io_t& rhs)const{
		return data_->fd() < rhs.data_fd();
	}
	bool operator==(const io_t& rhs)const{
		return data_->fd() == rhs.data_fd();
	}
	loop* get_loop(){return lo_;}
private:
	loop* const lo_;
	io(const io_t&);
	io_t& operator=(const io_t&);
	io();
};

template <typename T>
size_t hash_value(const io<T>& o){
	return hash_value(o.data_->fd());
}


#endif
