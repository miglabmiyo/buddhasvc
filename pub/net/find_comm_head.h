/*
 * find_comm_head.h
 *
 *  Created on: 2014年12月7日
 *      Author: kerry
 */

#ifndef FIND_COMM_HEAD_H_
#define FIND_COMM_HEAD_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{

class RecomFind:public LoginHeadPacket{
public:
	RecomFind(NetBase* m)
	:LoginHeadPacket(m){

	}
};

}

namespace netcomm_send{

class FindBook:public HeadPacket{
public:
	FindBook(){
		base_.reset(new netcomm_send::NetBase());
		hot_book_.reset(new base_logic::ListValue());
		scrip_book_.reset(new base_logic::ListValue());
		ritual_book_.reset(new base_logic::ListValue());
		theory_book_.reset(new base_logic::ListValue());
	}

	inline void set_book(const int64 id,const int32 type,const int32 attr,
			const std::string& name,const std::string& pic,const std::string& summary){
		scoped_ptr<base_logic::DictionaryValue> book(new base_logic::DictionaryValue());
		book->SetBigInteger(L"id",id);
		book->SetInteger(L"type",type);
		book->SetString(L"name",name);
		book->SetString(L"pic",pic);
		book->SetString(L"summary",summary);
		if(attr==0)
			hot_book_->Append(book.release());
		else if(attr==1)
			scrip_book_->Append(book.release());
		else if(attr==2)
			ritual_book_->Append(book.release());
		else if(attr==3)
			theory_book_->Append(book.release());
	}

	netcomm_send::NetBase* release(){
		if(!hot_book_->empty())
			this->base_->Set(L"hot",hot_book_.release());
		if(!scrip_book_->empty())
			this->base_->Set(L"scriptures",scrip_book_.release());
		if(!ritual_book_->empty())
			this->base_->Set(L"ritual",ritual_book_.release());
		if(!theory_book_->empty())
			this->base_->Set(L"theory",theory_book_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}

private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             hot_book_;
	scoped_ptr<base_logic::ListValue>             scrip_book_;
	scoped_ptr<base_logic::ListValue>             ritual_book_;
	scoped_ptr<base_logic::ListValue>             theory_book_;
};
}


#endif /* FIND_COMM_HEAD_H_ */
