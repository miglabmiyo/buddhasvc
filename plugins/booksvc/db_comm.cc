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

namespace booksvc_logic{

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

bool DBComm::OnGetBooksByType(const int32 type,std::list<booksvc_logic::BookBasicInfo>& list){
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

    //call buddha.proc_GetBooksByType(2)
	os<<"call buddha.proc_GetBooksByType("<<type<<");";
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
			booksvc_logic::BookBasicInfo book;
			if(rows[0]!=NULL)
				book.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				book.set_name(rows[1]);
			if(rows[2]!=NULL)
				book.set_pic(rows[2]);
			if(rows[3]!=NULL)
				book.set_summary(rows[3]);
			if(rows[4]!=NULL)
				book.set_fullurl(rows[4]);
			if(rows[5]!=NULL)
				book.set_freeurl(rows[5]);
			list.push_back(book);
		}
		return true;
	}

	return true;
}

bool DBComm::OnGetBookSummary(const int64 uid,const int64 bookid,int32& issave,
		booksvc_logic::BookBasicInfo& bookinfo){
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

    //call buddha.proc_GetBookSummary(2)
	os<<"call buddha.proc_GetBookSummary("<<bookid<<","<<uid<<");";
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
			bookinfo.set_id(bookid);
			if(rows[0]!=NULL)
				bookinfo.set_author(rows[0]);
			if(rows[0]!=NULL)
				bookinfo.set_pubtime(rows[1]);
			if(rows[2]!=NULL)
				bookinfo.set_chapter(atol(rows[2]));
			if(rows[3]!=NULL)
				bookinfo.set_summary(rows[3]);
			if(rows[4]!=NULL)
				bookinfo.set_fullurl(rows[4]);
			if(rows[5]!=NULL)
				bookinfo.set_freeurl(rows[5]);
			if(rows[6]!=NULL)
				issave = atol(rows[6]);
		}
		return true;
	}
	return true;
}

bool DBComm::OnGetBookList(const int64 uid,std::list<booksvc_logic::BookBasicInfo>& list){
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

    //call buddha.proc_GetOwnBook(10039)
	os<<"call buddha.proc_GetOwnBook("<<uid<<");";
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
			booksvc_logic::BookBasicInfo book;
			if(rows[0]!=NULL)
				book.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				book.set_type(atol(rows[1]));
			if(rows[2]!=NULL)
				book.set_name(rows[2]);
			if(rows[3]!=NULL)
				book.set_pic(rows[3]);
			if(rows[4]!=NULL)
				book.set_booktoken(rows[4]);
			/*if(rows[4]!=NULL)
				book.set_fullurl(rows[4]);
			if(rows[5]!=NULL)
				book.set_freeurl(rows[5]);*/
			list.push_back(book);
		}
		return true;
	}
	return false;
}


bool DBComm::OnWantGetBook(const int64 uid,const int64 bid,const std::string& token){
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

    //call buddha.proc_SetUserBookList(10039,9)
	os<<"call buddha.proc_SetUserBookList("<<uid<<","<<bid<<",\'"<<token<<"\');";
	std::string sql = os.str();
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}
	return true;
}

bool DBComm::OnGetBookChapters(const int64 bid,std::list<booksvc_logic::ChaptersInfo>& list){
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

    //call buddha.proc_GetBookChapter(6)
	os<<"call buddha.proc_GetBookChapter("<<bid<<");";
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
			booksvc_logic::ChaptersInfo chapter;
			if(rows[0]!=NULL)
				chapter.set_id(atoll(rows[0]));
			if(rows[1]!=NULL)
				chapter.set_bookid(atol(rows[1]));
			if(rows[2]!=NULL)
				chapter.set_chapter_url(rows[2]);
			if(rows[3]!=NULL)
				chapter.set_chapter_name(rows[3]);
			list.push_back(chapter);
		}
		return true;
	}
	return false;
}


}
