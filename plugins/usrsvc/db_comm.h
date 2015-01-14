/*
 * db_comm.h
 *
 *  Created on: 2014年12月4日
 *      Author: kerry
 */
#ifndef __BUDDHA__USRSVC__DB_COMM___
#define __BUDDHA__USRSVC__DB_COMM___
#include "user_basic_info.h"
#include "logic/logic_infos.h"
#include "config/config.h"
#include <list>
namespace usrsvc_logic{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();
public:
	static bool OnQuickRegister(usrsvc_logic::UserInfo& userinfo,
			base_logic::LBSInfos* lbsinfo);

	static bool OnThirdRegister(usrsvc_logic::UserInfo& userinfo,
			base_logic::LBSInfos* lbsinfo);
};

}
#endif


