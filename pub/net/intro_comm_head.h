/*
 * intro_comm_head.h
 *
 *  Created on: 2015年1月9日
 *      Author: kerry
 */

#ifndef INTRO_COMM_HEAD_H_
#define INTRO_COMM_HEAD_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>


namespace netcomm_recv{

//类别搜索
class IntroSearchType:public LoginHeadPacket{
public:
	IntroSearchType(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"btype",btype_);
		if(!r) error_code_ = BTYPE_LACK;
	}
	const int64 btype() {return this->btype_;}
private:
	int64 btype_;
};

//介绍详情
class IntroSummary:public LoginHeadPacket{
public:
	IntroSummary(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"introid",introid_);
		if(!r) error_code_ = BTYPE_LACK;
	}
	const int64 introid() {return this->introid_;}
private:
	int64 introid_;
};

}

namespace netcomm_send{

//介绍个人
class IntroInfo:public base_logic::DictionaryValue{
public:
	IntroInfo(){}
	virtual ~IntroInfo(){}
public:
	inline void set_basic_intro(const int64 id,const int32 type,const std::string& name,const std::string& pic,
			const std::string& summary){
		this->SetBigInteger(L"id",id);
		this->SetInteger(L"type",type);
		this->SetString(L"name",name);
		this->SetString(L"pic",pic);
		this->SetString(L"summary",summary);
	}
};

class IntroSearchType:public HeadPacket{
public:
	IntroSearchType(){
		base_.reset(new netcomm_send::NetBase());
		list_.reset(new base_logic::ListValue());
	}

	inline void SetIntroInfo(base_logic::DictionaryValue* build){
		list_->Append(build);
	}



	netcomm_send::NetBase* release(){
		if(!list_->empty())
			this->base_->Set(L"list",list_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             list_;
};

//思想和历史介绍
class IntroHisaThr:public HeadPacket{
public:
	IntroHisaThr(){
		base_.reset(new netcomm_send::NetBase());
		summary_.reset(new base_logic::DictionaryValue());
		chapter_list_.reset(new base_logic::ListValue());
	}

	virtual ~IntroHisaThr(){

	}

	inline void SetIntroSummary(const int32 chapter,
			const std::string& pic,const std::string& summary){
		this->summary_->SetInteger(L"chapter",chapter);
		this->summary_->SetString(L"pic",pic);
		this->summary_->SetString(L"summary",summary);
	}

	inline void SetChapterList(const std::string& name,const std::string& url,
			const int64 id){
		scoped_ptr<base_logic::DictionaryValue> unit(new base_logic::DictionaryValue());
		unit->SetString(L"name",name);
		unit->SetString(L"url",url);
		unit->SetBigInteger(L"index",id);
		chapter_list_->Append(unit.release());
	}

	netcomm_send::NetBase* release(){

		if(!summary_->empty())
			this->base_->Set(L"summary",summary_.release());
		if(!chapter_list_->empty())
			this->base_->Set(L"list",chapter_list_.release());

		head_->Set("result",base_.release());

		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::DictionaryValue>       summary_;
	scoped_ptr<base_logic::ListValue>             chapter_list_;
};

//艺术品
class IntroArt:public HeadPacket{
public:
	IntroArt(){
		base_.reset(new netcomm_send::NetBase());
		summary_.reset(new base_logic::DictionaryValue());
	}

	virtual ~IntroArt(){
	}

	netcomm_send::NetBase* release(){

		if(!summary_->empty())
			this->base_->Set(L"summary",summary_.release());
		head_->Set("result",base_.release());

		this->set_status(1);
		return head_.release();
	}

	inline void SetSummary(const std::string&pic,const std::string& summary){
		this->summary_->SetString(L"pic",pic);
		this->summary_->SetString(L"summary",summary);
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::DictionaryValue>       summary_;
};

}




#endif /* INTRO_COMM_HEAD_H_ */
