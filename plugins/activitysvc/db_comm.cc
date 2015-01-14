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

namespace activitysvc_logic{

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


bool DBComm::OnGainActivityList(const std::string& district,const std::string& city,
			std::list<activitysvc_logic::ActivityBasicInfo>& list){
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

    //call buddha.proc_GainActivityByAddress('成都市','下城区')
	os<<"call buddha.proc_GainActivityByAddress(\'"<<city<<"\',\'"<<district<<"\');";
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
			activitysvc_logic::ActivityBasicInfo activity;
			if(rows[0]!=NULL)
				activity.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				activity.set_name(rows[1]);
			if(rows[2]!=NULL)
				activity.set_pic(rows[2]);
			if(rows[3]!=NULL)
				activity.set_summary(rows[3]);
			if(rows[4]!=NULL)
				activity.set_longitude(atof(rows[4]));
			if(rows[5]!=NULL)
				activity.set_latitude(atof(rows[5]));
			list.push_back(activity);
		}
		return true;
	}
	return false;
}

bool DBComm::OnGainActivitySummary(const int64 aid,activitysvc_logic::ActivityBasicInfo& activityinfo){
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
	//aid = 1;

    //call buddha.proc_GainActivitySummary(1)
	os<<"call buddha.proc_GainActivitySummary("<<aid<<");";
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
				activityinfo.set_phone(rows[0]);
			if(rows[1]!=NULL)
				activityinfo.set_details(rows[1]);
			if(rows[2]!=NULL)
				activityinfo.set_address(rows[2]);
			if(rows[3]!=NULL)
				activityinfo.set_latitude(atof(rows[3]));
			if(rows[4]!=NULL)
				activityinfo.set_longitude(atof(rows[4]));
		}
		return true;
	}
	return false;
}

}
