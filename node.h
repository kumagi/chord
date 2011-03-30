#ifndef NODE_H
#define NODE_H

#include <vector>
#include <algorithm>

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include "rwsync.h"
#include "address.h"
#include "connection.h"

struct node_info{
	uint64_t id_;
	address ad_;
	node_info(uint64_t id, const address& ad)
		:id_(id), ad_(ad)
	{}
	bool operator<(const node_info& rhs)const{
		return id_ < rhs.id_;
	}
	bool operator==(const node_info& rhs)const{
		return (id_ == rhs.id_) && (ad_ == rhs.ad_);
	}
	node_info& operator=(const node_info& rhs){
		id_ = rhs.id_;
		ad_ = rhs.ad_;
		return *this;
	}
	const address& get_address()const{ return ad_;}
	const uint64_t& get_id()const{ return id_;}
	MSGPACK_DEFINE(id_,ad_);
};

class node : boost::noncopyable{
	uint64_t id;
	typedef std::vector<node_info> node_vector;
	typedef rwsync<node_vector> sync_node_vector;
	typedef sync_node_vector::read_ref read_ref;
	typedef sync_node_vector::upgrade_ref upgrade_ref;
	typedef sync_node_vector::write_ref write_ref;
	
	sync_node_vector finger_table; // always sorted
	sync_node_vector successor_table; // always sorted

public:
	const node_info& find_successor(uint64_t id, const address& origin){
		read_ref successor_table_r(successor_table); // read lock
		node_vector::const_iterator found 
			= std::lower_bound(successor_table_r->begin(),successor_table_r->end(), 
												 node_info(id,address()));
		if(found == successor_table_r->end()){
			assert(0);
		}
		if(found->get_id() == id){
			return *found;
		}else{ // relay it
			connection relay(found->get_address());
		}
	}
};


#endif
