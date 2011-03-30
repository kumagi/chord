#ifndef BEADED_BUFFER_H
#define BEADED_BUFFER_H

#define BEADS_GRAIN 1024
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <vector>
#include <stdexcept>
namespace detail{
enum {
	bead_length = BEADS_GRAIN - sizeof(void*)
};
class bead{ // this size must be BRADS_GRAIN
public:
	bead():next(NULL),buff(new uint8_t[bead_length]){}
	~bead(){
		delete[] buff;
	}
	bead* next;
	uint8_t* buff;
};
}

struct bead_ref{
	const uint8_t* const buff_;
	const size_t left_;
	bead_ref(const uint8_t* buff, size_t left)
		:buff_(buff),left_(left)
	{}
	const uint8_t* data()const{return buff_;}
	size_t left()const{return left_;}
};

class beaded_buffer{
public:
	beaded_buffer():head_offset(0),tail_offset(0),head(NULL),tail(NULL){}
	void append(const void* buff, size_t restsize){
		const uint8_t* const buff_p = reinterpret_cast<const uint8_t*>(buff);
		if(head == NULL){tail = head = new detail::bead();}
		int copied = 0;

		detail::bead* const backup_tail = tail;
		const size_t backup_tail_offset = tail_offset;
		std::vector<detail::bead*> alloc_ptr;
		try{
			while(restsize){
				const size_t bead_rest = detail::bead_length - tail_offset;
				const size_t copy_size = bead_rest < restsize ? bead_rest : restsize;
				memcpy(&tail->buff[tail_offset],
							 &buff_p[copied],
							 copy_size);
				copied += copy_size;
				tail_offset += copy_size;
				restsize -= copy_size;
				if(tail_offset == detail::bead_length){
					tail->next = new detail::bead();
					alloc_ptr.push_back(tail->next); // for exception
					tail = tail->next;
					tail->next = NULL;
					tail_offset = 0;
				}
			}
			assert(restsize == 0);
		}catch(std::bad_alloc){
			tail = backup_tail;
			tail_offset = backup_tail_offset;
			for(size_t i=0;i<alloc_ptr.size();++i){
				delete alloc_ptr[i];
			}
			throw std::bad_alloc();
		}
	}
	bead_ref get_next(){
		return bead_ref(&head->buff[head_offset], 
										head == tail ? tail_offset
										: detail::bead_length - head_offset);
	}
	bool is_remain()const{
		return !(head == tail && head_offset == tail_offset);
	}
	void forward(size_t t){
		while(0<t){
			const size_t reft = detail::bead_length - head_offset;
			const size_t gain = t < reft ? t : reft;
			t -= gain;
			head_offset += gain;
			if(head_offset == detail::bead_length){
				head_offset = 0;
				detail::bead* const next = head->next;
				delete head;
				head = next;
			}
			if(head == tail){
				assert(head_offset <= tail_offset);
			}
		}
	}
	~beaded_buffer(){
		detail::bead* ptr = head;
		while(ptr != NULL){
			detail::bead* next = ptr->next;
			delete ptr;
			ptr = next;
		}
	}
private:
	size_t head_offset,tail_offset; // moves between 0~bead_length
	detail::bead* head;
	detail::bead* tail;
};
#endif
