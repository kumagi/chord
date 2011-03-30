#include <gtest/gtest.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include "beaded_buffer.h"

TEST(construct , 1){
	beaded_buffer bb;
}
TEST(append, some){
	beaded_buffer bb;
	srand(0);
	std::vector<char> buf;
	for(int i=0;i<512;++i){
		buf.push_back(rand() & 255);
	}
	bb.append(buf.data(), buf.size());
}

TEST(append, check){
	beaded_buffer bb;
	std::vector<uint8_t> buf;
	for(int i=0;i<512;++i){
		buf.push_back(i & 255);
	}
	bb.append(buf.data(), buf.size());
	bead_ref ref = bb.get_next();
	for(size_t i=0;i<512;++i){
		EXPECT_EQ(ref.data()[i], i & 255);
	}
	EXPECT_TRUE(bb.is_remain());
}

TEST(append, more){
	beaded_buffer bb;
	const int testsize = 1020 * 512 + 432;
	srand(0);
	std::vector<uint8_t> buf;
	buf.reserve(testsize);
	for(int i=0;i<testsize;++i){
		buf.push_back(rand() & 255);
	}
	bb.append(buf.data(), buf.size());

	ASSERT_TRUE(bb.is_remain());
	srand(0);
	while(bb.is_remain()){
		const bead_ref ref = bb.get_next();
		for(size_t i=0;i<ref.left();++i){
			EXPECT_EQ(ref.data()[i], rand() & 255);
		}
		bb.forward(ref.left());
	}
}

TEST(append, many_times){
	beaded_buffer bb;
	const int testsize = 1020 * 512 + 432;
	const int grain = 823;
	int rest = testsize;
	srand(0);
	for(int i=0;i<512;++i){
		std::vector<uint8_t> buf;
		buf.reserve(grain);
		for(int i=0;i<grain;++i){
			buf.push_back(rand() & 255);
		}
		rest -= grain;
		bb.append(buf.data(), buf.size());
	}
	
	std::vector<uint8_t> buf;
	buf.reserve(testsize - rest);
	ASSERT_GT(rest, 0);
	
	while(rest > 0){
		buf.push_back(rand() & 255);
		rest--;
	}
	ASSERT_TRUE(bb.is_remain());
	
	srand(0);
	while(bb.is_remain()){
		const bead_ref ref = bb.get_next();
		for(size_t i=0;i<ref.left();++i){
			EXPECT_EQ(ref.data()[i], rand() & 255);
		}
		bb.forward(ref.left());
	}
}
