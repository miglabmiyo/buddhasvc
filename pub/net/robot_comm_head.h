/*
 * robot_comm.h
 *
 *  Created on: 2015年1月15日
 *      Author: mac
 */

#ifndef ROBOT_COMM_H_
#define ROBOT_COMM_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{

////添加基本信息
/*
class BookIntro:public LoginHeadPacket{
public:
	BookIntro(NetBase* m)
	:LoginHeadPacket(m){
		Init();
	}

	inline void Init(){
		bool r = false;
		r = m_->GetString(L"book_hash_name",&this->book_hash_name_);
		if(!r) error_code_ = BOOK_HASH_NAME_LACK;
		r = m_->GetString(L"intro",&this->intro_);
		if(!r) error_code_ = BOOK_INTRO_LACK;
		r = m_->GetInteger(L"btype",&this->btype_);
		if(!r) error_code_ = BTYPE_LACK;
		r = m_->GetString(L"book_name",&this->book_name_);
		if(!r) error_code_ = BOOK_NAME_LACK;
		r = m_->GetString(L"cover",&this->cover_);
		if(!r) error_code_ = BOOK_COVER_LACK;
		r = m_->GetString(L"pubtime",&this->pubtime_);
		if(!r) this->pubtime_ = "未知";
		r = m_->GetString(L"author",&this->author_);
		if(!r) this->author_ = "未知";

	}
private:
	int32                 btype_;
	std::string           book_hash_name_;
	std::string           book_name_;
	std::string           intro_;
	std::string           cover_;
	std::string           pubtime_;
	std::string           author_;
};*/

//书基本信息
class BookInfo: public LoginHeadPacket{
public:
	BookInfo(NetBase* m)
	:LoginHeadPacket(m){
		 Init();
	}

	inline void Init(){
		bool r = false;
		std::string book_name;
		r = m_->GetString(L"bookinfo_list",&this->bookinfo_list_);
		if(!r) error_code_ = BOOK_HASH_NAME_LACK;
	}
	const std::string& bookinfo_list() const {return this->bookinfo_list_;}
private:
	 std::string    bookinfo_list_;
};

//收录书的章节
class BookCollection: public LoginHeadPacket{
public:
	BookCollection(NetBase* m)
	:LoginHeadPacket(m){
		 Init();
	}

	inline void Init(){
		bool r = false;
		std::string book_name;
		r = m_->GetString(L"chapter_list",&this->book_chaptert_list_);
		if(!r) error_code_ = BOOK_HASH_NAME_LACK;
	}
	const std::string& book_chapter_list() const {return this->book_chaptert_list_;}
private:
	 std::string    book_chaptert_list_;
};
}

namespace netcomm_send{

class BookCollection:public HeadPacket{
public:
	BookCollection(){}
	virtual ~BookCollection(){}
public:
	netcomm_send::NetBase* release(){
		this->set_status(1);
		return head_.release();
	}
};
}




#endif /* ROBOT_COMM_H_ */
