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

namespace findsvc_logic{

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

bool DBComm::GetBookStoreFind(std::list<findsvc_logic::FindBookInfo>& list){
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

    //call buddha.proc_GetBookStoreFind()
	os<<"call proc_GetBookStoreFind();";
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
			findsvc_logic::FindBookInfo book;
			if(rows[0]!=NULL)
				book.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				book.set_type(atol(rows[1]));
			if(rows[2]!=NULL)
				book.set_name(rows[2]);
			if(rows[3]!=NULL)
				book.set_pic(rows[3]);
			if(rows[4]!=NULL)
				book.set_summary(rows[4]);
			if(rows[5]!=NULL)
				book.set_attr(atol(rows[5]));
			list.push_back(book);
		}
		return true;
	}

	return true;
}


}
