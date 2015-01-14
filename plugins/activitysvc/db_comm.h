/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__ACTIVITYSVC__DB_COMM___
#define __BUDDHA__ACTIVITYSVC__DB_COMM___
#include "activity_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace activitysvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();
public:
	static bool OnGainActivityList(const std::string& district,const std::string& city,
			std::list<activitysvc_logic::ActivityBasicInfo>& list);

	static bool OnGainActivitySummary(const int64 aid,activitysvc_logic::ActivityBasicInfo& activityinfo);

};

}

#endif

