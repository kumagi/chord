#ifndef PEAKY_EXTEND_BUFFER_H
#define PEAKY_EXTEND_BUFFER_H
#include <stdint.h>
#include <stdlib.h>
#include "buffer_ref.h"

class peaky_extend_buffer{
public:
	peaky_extend_buffer(const int default_length = 32)
		:length_(default_length),used_(0)
		,buff_(reinterpret_cast<uint8_t*>(malloc(default_length)))
	{}
	void extend(){
		length_ <<= 2;
		buff_ = reinterpret_cast<uint8_t*>(realloc(buff_, length_));
	}
	uint8_t* get_next(){return &buff_[used_]; }
	const uint8_t* get_next()const{return &buff_[used_]; }
	bool consume(const int consumed){
		if(used_ + consumed < length_){
			used_ += consumed;
			return true;
		}else{
			return false;
		}
	}
	uint8_t* data(){return buff_;}
	const uint8_t* data()const{return buff_;}
	uint64_t used()const{return used_;}
	uint64_t left()const{return length_ - used_;}
	bool is_filled()const{return length_ == used_;}
	~peaky_extend_buffer(){free(buff_);}
private:
	uint64_t length_;
	uint64_t used_;
	uint8_t* buff_;
};


#endif
