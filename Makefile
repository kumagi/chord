CXX= g++
OPTS= -O0 -g
PROF=-pg -fno-inline
WARNS=-W -Wall -Wextra -Wformat=2 -Wstrict-aliasing=4 -Wcast-qual -Wcast-align \
	-Wwrite-strings -Wfloat-equal -Wpointer-arith -Wswitch-enum
TEST_LD= -lpthread -lboost_thread
THREAD_LD= -lpthread -lboost_thread
GTEST_INC= -I$(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_DIR=/opt/google/gtest-1.5.0

NOTIFY=&& notify-send Test success! -i ~/themes/ok_icon.png || notify-send Test failed... -i ~/themes/ng_icon.png

#target:nodetest
#target:chord
#target:beadtest
#target:eventtest
target:connectiontest
#target:buffertest
target:accepttest

chord:chord.o
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD)
chord.o: chord.cc
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

nodetest:nodetest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD)
nodetest.o:nodetest.cc node.h
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

accepttest:accepttest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD) -lev
accepttest.o:accepttest.cc acceptor.h
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

eventtest:eventtest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD) -lev
	./$@  $(NOTIFY)
eventtest.o:eventtest.cc event.h
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

beadtest:beadtest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD)
	./$@ $(NOTIFY)
beadtest.o:beadtest.cc beaded_buffer.h
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

buffertest:buffertest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD)
	./$@ $(NOTIFY)
buffertest.o:buffertest.cc beaded_buffer.h peaky_extend_buffer.h
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

connectiontest:connectiontest.o gtest_main.a
	$(CXX) $^ -o $@ $(OPTS) $(WARNS) $(THREAD_LD) -lev
	./$@ $(NOTIFY)
connectiontest.o:connectiontest.cc connection.h beaded_buffer.h peaky_extend_buffer.h buffered_connection.h 
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)

#	$(CXX) -c -o $@ sl.cc  $(OPTS) $(WARNS) -I$(GTEST_DIR)/include -I$(GTEST_DIR)
# gtest
gtest_main.o:
	$(CXX) $(GTEST_INC) -c $(OPTS) $(GTEST_DIR)/src/gtest_main.cc -o $@
gtest-all.o:
	$(CXX) $(GTEST_INC) -c $(OPTS) $(GTEST_DIR)/src/gtest-all.cc -o $@
gtest_main.a:gtest-all.o gtest_main.o
	ar -r $@ $^

clean:
	rm -f *.o
	rm -f *~
