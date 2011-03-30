#ifndef LOOP_H
#define LOOP_H
#include <assert.h>
#include <stdexcept>
#include <ev.h>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

typedef struct ev_loop ev_loop_t;
namespace detail{
void runner(ev_loop_t* const ev){
	assert(ev);
	ev_loop(ev, 0);
	std::cout << "done" << std::endl;
}
}

class loop{
public:
	loop()
		:loop_(ev_loop_new(EVFLAG_AUTO)){
		if(!loop_){throw std::bad_alloc();}
	}
	~loop(){
		if(loop_){ev_loop_destroy(loop_);}
	}

	void start(int threadnum){
		for(int i=0; i<threadnum; ++i){
			threads_.create_thread(boost::bind(detail::runner, loop_));
		}
	}
	void run(int once = 0){
		ev_loop(loop_, once == 0 ? 0 : EVLOOP_ONESHOT);
	}
	void join(){
		threads_.join_all();
	}

	ev_loop_t* get_loop(){return loop_;}
	
private:
	boost::thread_group threads_;
	ev_loop_t* const loop_;
	ev_io io_;
};


#endif
