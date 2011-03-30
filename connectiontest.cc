#include <gtest/gtest.h>
#include <unistd.h>
#include <string.h>
#include "event.h"
#include "connection.h"
#include "buffered_connection.h"

TEST(construct,by_pipe){
	int fds[2];
	ASSERT_TRUE(pipe(fds) == 0);
	connection reader(fds[0]);
	connection writer(fds[1]);
	writer.write("hogehoge",8);

	char buff[10]; 
	reader.read(buff,8);
	
	ASSERT_TRUE(strncmp(buff,"hogehoge",8) == 0);
}

TEST(accept, 1){
	
}
