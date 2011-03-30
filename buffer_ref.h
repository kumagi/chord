#ifndef BUFFER_REF_H
#define BUFFER_REF_H
#include <stdint.h>
// it has no duty
struct buffer_ref{
	const uint8_t* const data_;
	uint64_t size_;
	buffer_ref(const uint8_t* const buff, const uint64_t size)
		:data_(buff),size_(size) {}
};

#endif
