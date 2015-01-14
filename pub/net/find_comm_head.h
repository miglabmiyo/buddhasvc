/*
 * find_comm_head.h
 *
 *  Created on: 2014年12月7日
 *      Author: kerry
 */

#ifndef FIND_COMM_HEAD_H_
#define FIND_COMM_HEAD_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{

class RecomFind:public LoginHeadPacket{
public:
	RecomFind(NetBase* m)
	:LoginHeadPacket(m){

	}
};

}



#endif /* FIND_COMM_HEAD_H_ */
