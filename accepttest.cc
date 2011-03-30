#include <gtest/gtest.h>
#include "acceptor.h"
#include "util.h"

TEST(construct,a){
	loop l;
	address myad(aton("0.0.0.0"), 12);
	acceptor a(myad, &l);
}
