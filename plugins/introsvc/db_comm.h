/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__INTROSVC__DB_COMM___
#define __BUDDHA__INTROSVC__DB_COMM___
#include "intro_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace introsvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();

	static bool SearchIntroInfoByType(const int32 type,std::list<introsvc_logic::IntroBasicInfo>& list);

	static bool GetIntroSummary(const int64 id,introsvc_logic::IntroBasicInfo& introinfo);

	static bool GetIntroChapter(const int64 id,std::list<introsvc_logic::IntroChapterInfo>& list);

public:

};

}

#endif

