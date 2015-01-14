/*
 * building_comm_head.h
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#ifndef _NET_BUILDING_COMM_HEAD_H_
#define _NET_BUILDING_COMM_HEAD_H_

#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>


namespace netcomm_recv{

//检索周围
class RetrievalNearBuilding: public LoginHeadPacket{
public:
	RetrievalNearBuilding(NetBase* m)
	:LoginHeadPacket(m){

	}
};

//根据类别搜索
class SearchTypeBuilding:public LoginHeadPacket{
public:
	SearchTypeBuilding(NetBase* m)
	  :LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"btype",btype_);
		if(!r) error_code_ = BTYPE_LACK;
		GETBIGINTTOINT(L"from",from_);
		if(!r) from_ = 0;
		GETBIGINTTOINT(L"count",count_);
		if(!r) count_ =10;
	}
public:
	const int32 btype() const {return this->btype_;}
	const int32 from() const {return this->from_;}
	const int32 count() const {return this->count_;}
private:
	int32       btype_;
	int32       from_;
	int32       count_;
};

//获取详情及其它相关
class GainBuildingSummary:public LoginHeadPacket{
public:
	GainBuildingSummary(NetBase* m)
		:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"uid",uid_);
		if(!r) error_code_ = BTYPE_LACK;
		GETBIGINTTOINT(L"bid",bid_);
		if(!r) error_code_ = BUILD_ID_LACK;
	}
public:
	const int32 uid() const {return this->uid_;}
	const int32 bid() const {return this->bid_;}
private:
	int64   uid_;
	int64   bid_;
};

}



namespace netcomm_send{

class BaseBuilding:public base_logic::DictionaryValue{
public:
	BaseBuilding(){
	}
	virtual ~BaseBuilding(){}
public:
	inline void set_basic(const int64 id,const std::string& name,const int32 type,const std::string& pic){
		scoped_ptr<base_logic::DictionaryValue> basic(new base_logic::DictionaryValue());
		basic->SetBigInteger(L"id",id);
		basic->SetString(L"name",name);
		basic->SetInteger(L"type",type);
		basic->SetString(L"pic",pic);
		this->Set(L"basic",basic.release());
	}
	inline void set_location(const std::string& address,const double latitue,const double longitude,
			const std::string& city,const double distance){
		scoped_ptr<base_logic::DictionaryValue> location(new base_logic::DictionaryValue());
		location->SetString(L"address",address);
		location->SetReal(L"latitude",latitue);
		location->SetReal(L"longitude",longitude);
		location->SetString(L"city",city);
		location->SetReal(L"distance",distance);
		this->Set(L"location",location.release());
	}
};

class RetrievalNearBuilding:public HeadPacket{
public:
	RetrievalNearBuilding(){
		base_.reset(new netcomm_send::NetBase());
		near_builder_.reset(new base_logic::ListValue());
	}

	inline void SetBuildingInfo(base_logic::DictionaryValue* build){
		near_builder_->Append(build);
	}

	inline void SetBuildingCount(const int32 count){
		this->base_->SetInteger(L"count",count);
	}

	netcomm_send::NetBase* release(){
		if(!near_builder_->empty())
			this->base_->Set(L"nearbuild",near_builder_.release());
		//this->location_->Set(L"weather",weather_.release());
		//this->location_->Set(L"city",city_.release());
		//this->location_->Set(L"temp",temp_.release());
		head_->Set("result",base_.release());

		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             near_builder_;
};

class BuildingBaseSummary:public base_logic::DictionaryValue{
public:
	BuildingBaseSummary(){}
	virtual ~BuildingBaseSummary(){}
public:
	inline void set_summary(const std::string& summary){
		this->SetString(L"summary",summary);
	}
	inline void set_phone(const std::string& phone){
		this->SetString(L"phone",phone);
	}
};

class BuildAboutUser:public base_logic::DictionaryValue{
public:
	BuildAboutUser(){}
	virtual ~BuildAboutUser(){}
public:
	inline void set_collect(const int32 collect){
		this->SetInteger(L"collect",collect);
	}
};

class BuildingSummary:public HeadPacket{
public:
	BuildingSummary(){
		base_.reset(new netcomm_send::NetBase());
		summary_.reset(new base_logic::DictionaryValue());
		user_.reset(new base_logic::DictionaryValue());
	}
	virtual ~BuildingSummary(){}

	netcomm_send::NetBase* release(){
		if(!summary_->empty())
			this->base_->Set(L"summary",summary_.release());
		if(!user_->empty())
			this->base_->Set(L"user",user_.release());
		//this->location_->Set(L"weather",weather_.release());
		//this->location_->Set(L"city",city_.release());
		//this->location_->Set(L"temp",temp_.release());
		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
public:
	inline void set_summary(base_logic::DictionaryValue* build){
		summary_.swap(build);
	}

	inline void set_user(base_logic::DictionaryValue* user){
		user_.swap(user);
	}
private:
	scoped_ptr<netcomm_send::NetBase>                     base_;
	scoped_ptr<base_logic::DictionaryValue>             summary_;
	scoped_ptr<base_logic::DictionaryValue>             user_;
};

}




#endif /* BUILDING_COMM_HEAD_H_ */
