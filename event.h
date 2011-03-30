#ifndef EVENT_H
#define EVENT_H
#include <assert.h>
#include <stdexcept>
#include <ev.h>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include "loop.h"

class event{
public:
	void start(int threadnum){lo_.start(threadnum);}
	void run(int once=0){lo_.run(once);}
	void join(){lo_.join();}
private:
	loop lo_;
};
#endif
