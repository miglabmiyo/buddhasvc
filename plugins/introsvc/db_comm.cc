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

namespace introsvc_logic{

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

bool DBComm::SearchIntroInfoByType(const int32 type,
		std::list<introsvc_logic::IntroBasicInfo>& list){
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

    //call buddha.proc_GetIntroInfoByType(4)
	os<<"call buddha.proc_GetIntroInfoByType("<<type<<");";
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
			introsvc_logic::IntroBasicInfo introinfo;
			if(rows[0]!=NULL)
				introinfo.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				introinfo.set_type(atol(rows[1]));
			if(rows[2]!=NULL)
				introinfo.set_name(rows[2]);
			if(rows[3]!=NULL)
				introinfo.set_pic(rows[3]);
			if(rows[4]!=NULL)
				introinfo.set_summary(rows[4]);
			list.push_back(introinfo);
		}
		return true;
	}
	return false;
}

bool DBComm::GetIntroSummary(const int64 id,introsvc_logic::IntroBasicInfo& introinfo){
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

    //call buddha.proc_GetInfoSummary(11)
	os<<"call buddha.proc_GetInfoSummary("<<id<<");";
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
				introinfo.set_content(rows[0]);
			if(rows[1]!=NULL)
				introinfo.set_big_pic(rows[1]);
		}
		return true;
	}
	return false;
}

bool DBComm::GetIntroChapter(const int64 id,std::list<introsvc_logic::IntroChapterInfo>& list){
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

    //call buddha.proc_GetHisThrChapter(1)
	os<<"call buddha.proc_GetHisThrChapter("<<id<<");";
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
			introsvc_logic::IntroChapterInfo info;
			if(rows[0]!=NULL)
				info.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				info.set_bid(atoll(rows[1]));
			if(rows[2]!=NULL)
				info.set_chapter_url(rows[2]);
			if(rows[3]!=NULL)
				info.set_name(rows[3]);
			list.push_back(info);
		}
		return true;
	}
	return false;
}

}
