#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

namespace detail{
inline uint64_t murmur_hash(const void* const buf, size_t size) {
  assert(buf);
  const static uint64_t mul = 0xc6a4a7935bd1e995ULL;
  const static int32_t rtt = 47;
  uint64_t hash = 19780211ULL ^ (size * mul);
  const unsigned char* rp = (const unsigned char*)buf;
  while (size >= sizeof(uint64_t)) {
    uint64_t num = ((uint64_t)rp[0] << 0) | ((uint64_t)rp[1] << 8) |
      ((uint64_t)rp[2] << 16) | ((uint64_t)rp[3] << 24) |
      ((uint64_t)rp[4] << 32) | ((uint64_t)rp[5] << 40) |
      ((uint64_t)rp[6] << 48) | ((uint64_t)rp[7] << 56);
    num *= mul;
    num ^= num >> rtt;
    num *= mul;
    hash *= mul;
    hash ^= num;
    rp += sizeof(uint64_t);
    size -= sizeof(uint64_t);
  }
  switch (size) {
	case 7: hash ^= (uint64_t)rp[6] << 48;
	case 6: hash ^= (uint64_t)rp[5] << 40;
	case 5: hash ^= (uint64_t)rp[4] << 32;
	case 4: hash ^= (uint64_t)rp[3] << 24;
	case 3: hash ^= (uint64_t)rp[2] << 16;
	case 2: hash ^= (uint64_t)rp[1] << 8;
	case 1: hash ^= (uint64_t)rp[0];
		hash *= mul;
  };
  hash ^= hash >> rtt;
  hash *= mul;
  hash ^= hash >> rtt;
  return hash;
}
}

#define hash_value_define(X)										\
	uint64_t hash_value(const X u) __attribute__((always_inline));	\
	uint64_t hash_value(const X u){								\
		return detail::murmur_hash(&u,sizeof(X));		\
	}

hash_value_define(char);
hash_value_define(uint64_t);
hash_value_define(uint32_t);
hash_value_define(uint16_t);
hash_value_define(uint8_t);
hash_value_define(int64_t);
hash_value_define(int32_t);
hash_value_define(int16_t);
hash_value_define(int8_t);
hash_value_define(long);
#undef hash_value_define

#include <string>
uint64_t hash_value(const std::string& str){
	return detail::murmur_hash(str.data(),str.length());
}


#define define_hash_value_iter(X)													\
	template <typename T>																		\
	uint64_t hash_value(const X<T>& vec){										\
		typedef typename X<T>::const_iterator iter_t;					\
		iter_t end = vec.end();																\
		uint64_t num = 0ULL;																	\
		for(iter_t iter = vec.begin(); iter != end; ++iter){	\
			num += hash_value(*iter);														\
		}																											\
		return num;																						\
	}
#include <vector>
#include <set>
#include <boost/array.hpp>
define_hash_value_iter(std::vector);
define_hash_value_iter(std::set);
define_hash_value_iter(std::multiset);
#undef define_hash_value_iter

#define define_hash_value_pair(X)														\
	template <typename K, typename V>													\
	uint64_t hash_value(const X<K, V>& vec){									\
		typedef typename X<K,V>::const_iterator iter_t;					\
		iter_t end = vec.end();																	\
		uint64_t num = 0ULL;																		\
		for(iter_t iter = vec.begin(); iter != end; ++iter){		\
			num += hash_value(iter->first);												\
			num += hash_value(iter->second);											\
		}																												\
		return num;																							\
	}
#include <map>
define_hash_value_pair(std::map);
define_hash_value_pair(std::multimap);
#undef define_hash_value_pair

#include <arpa/inet.h>

int aton(const char* ipaddress){
	struct in_addr tmp_inaddr;
	int ip = 0;
	if(inet_aton(ipaddress,&tmp_inaddr)){
		ip = tmp_inaddr.s_addr;
	}else {
		printf("aton:address invalid\n");
	}
	return ip;
}
#endif
