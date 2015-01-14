/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__BOOKSVC__DB_COMM___
#define __BUDDHA__BOOKSVC__DB_COMM___
#include "book_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace booksvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();
	//根据类别查询
	static bool OnGetBooksByType(const int32 type,std::list<booksvc_logic::BookBasicInfo>& list);
	//根据ID查询
	static bool OnGetBookSummary(const int64 uid,const int64 bookid,int32& issave,
			booksvc_logic::BookBasicInfo& bookinfo);
	//获取书单
	static bool OnGetBookList(const int64 uid,std::list<booksvc_logic::BookBasicInfo>& list);
	//获得此书
	static bool OnWantGetBook(const int64 uid,const int64 bid,const std::string& token);

	//获得此书的章节描述
	static bool OnGetBookChapters(const int64 bid,std::list<booksvc_logic::ChaptersInfo>& list);
public:

};

}

#endif

