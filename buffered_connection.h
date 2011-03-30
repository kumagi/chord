#ifndef BUFFERED_CONNECTION_H
#define BUFFERED_CONNECTION_H
#include "connection.h"
#include "beaded_buffer.h"
#include "peaky_extend_buffer.h"
#include <boost/noncopyable.hpp>
#include <vector>
#include <algorithm>
#include "buffer_ref.h"

class buffered_connection : private connection, private io_base{
public:
	using connection::fd;
	int fd()const{return connection::fd();}
	buffered_connection(const address& target)
		:connection(target),consumed(0){}
	buffered_connection(const int fd)
		:connection(fd),consumed(0){}
	void write(const void* buff, int size)throw(std::bad_alloc){
		int rest = size;
		int offset = 0;
		if(!write_buff.is_remain()){
			const size_t sent = connection::write(buff,size);
			rest -= sent;
			offset += sent;
		}
		if(0 < rest){
			const char* const buff_p = static_cast<const char*>(buff);
			write_buff.append(&buff_p[offset], rest);
		}
	}
	int copy_read(char* const buff, const uint64_t length){
		const uint64_t copy_length = std::min(read_buff.used() - consumed, length);
		const uint8_t* rbuffer = &read_buff.data()[consumed];
		memcpy(buff, rbuffer, copy_length);
		return copy_length;
	}
	buffer_ref copy_ref()const{
		const uint64_t length = read_buff.used() - consumed;
		return buffer_ref(read_buff.get_next(), length);
	}
	
	
	bool is_sent()const{
		return !write_buff.is_remain();
	}
	bool on_readable(){
		while(1){
			const int readlen = connection::read(read_buff.get_next(), read_buff.left());
			if(readlen < 0){ break; }
			if(readlen == 0){ return false;}
			read_buff.consume(readlen);
			if(read_buff.left() == 0){read_buff.extend();}
		}
		return true;
	}
	bool on_writable(){
		while(1){
			const int writelen = connection::write(read_buff.get_next(), read_buff.left());
			if(writelen < 0){ break; }
			write_buff.forward(writelen);
			if(writelen == 0){ return false;}
		}
		return true;
	}
	virtual ~buffered_connection(){}
private:
	
	peaky_extend_buffer read_buff;
	uint64_t consumed;
	beaded_buffer write_buff;
};

#endif
