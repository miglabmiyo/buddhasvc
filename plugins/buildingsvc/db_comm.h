/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__BUILDINGSVC__DB_COMM___
#define __BUDDHA__BUILDINGSVC__DB_COMM___
#include "building_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace buildingsvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();
	static bool RetrievalNearBuilding(const std::string& district,const std::string& city,
			std::list<base_logic::BuildingBasicInfo>& list);
	static bool SearchBuildingByType(const int32 type,const int32 from,const int32 count,
			std::list<base_logic::BuildingBasicInfo>& list);

	static bool GainBuildingSummary(const int64 uid,const int64 bid,std::string& phone,
			int32& collection,std::string& summary);
public:

};

}

#endif

