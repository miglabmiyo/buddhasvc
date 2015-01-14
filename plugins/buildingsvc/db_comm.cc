/*
 * db_comm.cc
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#include "db_comm.h"
#include "db/base_db_mysql_auto.h"
#include <sstream>
#include <mysql.h>

namespace buildingsvc_logic{

void DBComm::Init(std::list<base::ConnAddr>& addrlist){
#if defined (_DB_POOL_)
	base_db::MysqlDBPool::Init(addrlist);
#endif
}

void DBComm::Dest(){
#if defined (_DB_POOL_)
	base_db::MysqlDBPool::Dest();
#endif
}


bool DBComm::RetrievalNearBuilding(const std::string& district,const std::string& city,
			std::list<base_logic::BuildingBasicInfo>& list){
	bool r = false;
#if defined (_DB_POOL_)
	base_db::AutoMysqlCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	MYSQL_ROW rows;

	if (engine==NULL){
		LOG_ERROR("GetConnection Error");
		return false;
	}

    //call buddha.proc_GainBuildInfoByAddress('杭州市','西湖区')
	os<<"call buddha.proc_GainBuildInfoByAddress(\'"<<city<<"\',\'"<<district<<"\');";
	std::string sql = os.str();
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}


	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::BuildingBasicInfo building;
			if(rows[0]!=NULL)
				building.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				building.set_name(rows[1]);
			if(rows[2]!=NULL)
				building.set_type(atol(rows[2]));
			if(rows[3]!=NULL)
				building.set_pic(rows[3]);
			if(rows[4]!=NULL)
				building.set_address(rows[4]);
			if(rows[5]!=NULL)
				building.set_latitude(atof(rows[5]));
			if(rows[6]!=NULL)
				building.set_longitude(atof(rows[6]));
			if(rows[7]!=NULL)
				building.set_city(rows[7]);
			if(rows[8]!=NULL)
				building.set_district(rows[8]);
			list.push_back(building);
		}
		return true;
	}
	return false;
}

bool DBComm::SearchBuildingByType(const int32 type,const int32 from,const int32 count,
			std::list<base_logic::BuildingBasicInfo>& list){
	bool r = false;
#if defined (_DB_POOL_)
	base_db::AutoMysqlCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	MYSQL_ROW rows;

	if (engine==NULL){
		LOG_ERROR("GetConnection Error");
		return false;
	}

    //call buddha.proc_GainBuildInfoByType(5,0,0)
	os<<"call buddha.proc_GainBuildInfoByType("<<type<<","<<from<<","<<count<<");";
	std::string sql = os.str();
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}


	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::BuildingBasicInfo building;
			if(rows[0]!=NULL)
				building.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				building.set_name(rows[1]);
			if(rows[2]!=NULL)
				building.set_type(atol(rows[2]));
			if(rows[3]!=NULL)
				building.set_pic(rows[3]);
			if(rows[4]!=NULL)
				building.set_address(rows[4]);
			if(rows[5]!=NULL)
				building.set_latitude(atof(rows[5]));
			if(rows[6]!=NULL)
				building.set_longitude(atof(rows[6]));
			if(rows[7]!=NULL)
				building.set_city(rows[7]);
			if(rows[8]!=NULL)
				building.set_district(rows[8]);
			list.push_back(building);
		}
		return true;
	}
	return false;
}

bool DBComm::GainBuildingSummary(const int64 uid,const int64 bid,std::string& phone,
		int32& collection,std::string& summary){
	bool r = false;
#if defined (_DB_POOL_)
	base_db::AutoMysqlCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	MYSQL_ROW rows;

	if (engine==NULL){
		LOG_ERROR("GetConnection Error");
		return false;
	}
	bid = 1;

    //call buddha.proc_GainGetBuildingSummary(2,10039)
	os<<"call buddha.proc_GainGetBuildingSummary("<<bid<<","<<uid<<");";
	std::string sql = os.str();
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}


	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			if(rows[0]!=NULL)
				phone = rows[0];
			if(rows[1]!=NULL)
				collection = atol(rows[1]);
			if(rows[2]!=NULL)
				summary = rows[2];
		}
		return true;
	}
	return false;
}

}
