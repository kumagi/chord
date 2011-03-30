#include "peaky_extend_buffer.h"
#include "beaded_buffer.h"
#include <gtest/gtest.h>

TEST(peaky, left){
	peaky_extend_buffer peb;
	uint8_t* buf = peb.get_next();
	ASSERT_EQ(peb.left(), 32);
	std::cout << "used:" << peb.used() << std::endl
						<< "left:" << peb.left() << std::endl;
	for(int i=0;i<32;i++){
		buf[i] = i;
		ASSERT_EQ(peb.left(), 32-i);
		peb.consume(1);
	}
}

