/*
 * activtiy_comm_head.h
 *
 *  Created on: 2015年1月7日
 *      Author: kerry
 */

#ifndef ACTIVTIY_COMM_HEAD_H_
#define ACTIVTIY_COMM_HEAD_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{

//活动详情
class ActivtiySummary: public LoginHeadPacket{
public:
	ActivtiySummary(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		Init();
	}

	inline void Init(){
		bool r = false;
		GETBIGINTTOINT(L"aid",aid_);
		if(!r) error_code_ = ACTIVITY_ID_LACK;
	}

	const int64 aid(){return this->aid_;}
private:
	int64 aid_;
};

//获取活动首页
class Activties:public LoginHeadPacket{
public:
	Activties(NetBase* m)
	:LoginHeadPacket(m){
	}
};

}

namespace netcomm_send{
//活动详情
class ActivtiySummary: public HeadPacket{
public:
	ActivtiySummary(){
		base_.reset(new netcomm_send::NetBase());
		activtiy_summary_.reset(new base_logic::DictionaryValue());
		location_summary_.reset(new base_logic::DictionaryValue());
	}

	virtual ~ActivtiySummary(){

	}

	netcomm_send::NetBase* release(){
		if(activtiy_summary_->size()!=0)
			this->base_->Set(L"summary",activtiy_summary_.release());
		if(location_summary_->size()!=0)
			this->base_->Set(L"location",location_summary_.release());
		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}

	void set_activtiy(const std::string& phone,const std::string& summary){
		activtiy_summary_->SetString(L"phone",phone);
		activtiy_summary_->SetString(L"summary",summary);
	}

	void set_location(const std::string& address,const double latitue,const double longitude){
		location_summary_->SetString(L"address",address);
		location_summary_->SetReal(L"latitude",latitue);
		location_summary_->SetReal(L"longitude",longitude);
	}

private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::DictionaryValue>       activtiy_summary_;
	scoped_ptr<base_logic::DictionaryValue>       location_summary_;
};


class Activties:public HeadPacket{
public:
	Activties(){
		base_.reset(new netcomm_send::NetBase());
		recomm_activties_.reset(new base_logic::ListValue());
		comm_activties_.reset(new base_logic::ListValue());
	}

	netcomm_send::NetBase* release(){
		if(!recomm_activties_->empty())
			this->base_->Set(L"recomm",recomm_activties_.release());
		if(!comm_activties_->empty())
			this->base_->Set(L"common",comm_activties_.release());
		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}

	inline void SetActivtiesInfo(const int64 id,const std::string& name,
			const std::string& pic,const int32 flag){
		scoped_ptr<base_logic::DictionaryValue> activtiy(new base_logic::DictionaryValue());
		activtiy->SetBigInteger(L"id",id);
		activtiy->SetString(L"name",name);
		activtiy->SetString(L"pic",pic);
		if(flag==0)
			recomm_activties_->Append(activtiy.release());
		else
			comm_activties_->Append(activtiy.release());
	}


private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             recomm_activties_;
	scoped_ptr<base_logic::ListValue>             comm_activties_;
};
}


#endif /* ACTIVTIY_COMM_HEAD_H_ */
