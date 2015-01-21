/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__FINDSVC__DB_COMM___
#define __BUDDHA__FINDSVC__DB_COMM___
#include "find_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace findsvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();

	static bool GetBookStoreFind(std::list<findsvc_logic::FindBookInfo>& list);

};

}

#endif

