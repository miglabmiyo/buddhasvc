/*
 * comm_head.h
 *
 *  Created on: 2014年11月17日
 *      Author: kerry
 */

#ifndef _NET_COMM_HEAD_H_
#define _NET_COMM_HEAD_H_

#include "basic/basictypes.h"
#include "base/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>

#define NET_VALUE  scoped_ptr<base::Value>
/*******接收相关************/
namespace netcomm_recv{

class NetBase:public base::DictionaryValue{
public:
	//virtual bool Deserialize(const std::string& rest_str);
};
//快速登錄
class QucikLogin:public NetBase{
public:
	QucikLogin(const std::string& imei,const std::string& machine,const std::string&
			latitude,const std::string& longitude)
		:imei_(base::Value::CreateNullValue())
		,machine_(base::Value::CreateNullValue())
		,latitude_(base::Value::CreateNullValue())
		,longitude_(base::Value::CreateStringValue(longitude)){
		this->Set("imei",imei_);
		this->Set("machine",machine_);
		this->Set("latitude",latitude_);
		this->Set("longitude",longitude_);
	}



private:
	NET_VALUE  imei_;
	NET_VALUE  machine_;
	NET_VALUE  latitude_;
	NET_VALUE  longitude_;
};

//第三方登錄
class ThirdLogin:public NetBase{
public:
	ThirdLogin(const std::strng& machine,const std::string& nickname,const std::string& source,
			const std::string& session,const std::string& sex,const std::string& birthday,
			const std::string& location,const std::string& head,const std::string&
			latitude,const std::string& longitude)
		:machine_(base::Value::CreateStringValue(machine))
		,nickname_(base::Value::CreateStringValue(nickname))
		,source_(base::Value::CreateStringValue(source))
		,session_(base::Value::CreateStringValue(session))
		,sex_(base::Value::CreateStringValue(sex))
		,birthday_(base::Value::CreateStringValue(birthday))
		,location_(base::Value::CreateStringValue(location))
		,head_(base::Value::CreateStringValue(head))
		,latitude_(base::Value::CreateStringValue(latitude))
		,longitude_(base::Value::CreateStringValue(longitude)){

		this->Set("machine",machine_);
		this->Set("nickname",nickname_);
		this->Set("source",source_);
		this->Set("session",session_);
		this->Set("sex",sex_);
		this->Set("birthday",birthday_);
		this->Set("location",location_);
		this->Set("head",head_);
		this->Set("latitude",latitude_);
		this->Set("longitude",longitude_);

	}
private:
	NET_VALUE   machine_;
	NET_VALUE   nickname_;
	NET_VALUE   source_;
	NET_VALUE   session_;
	NET_VALUE   sex_;
	NET_VALUE   birthday_;
	NET_VALUE   location_;
	NET_VALUE   head_;
	NET_VALUE   latitude_;
	NET_VALUE   longitude_;
};

//百度推送綁定
class BDBindPush: public NetBase{
public:
	BDBindPush(const std::string& platform,const std::string& uid,const std::string& token,
			const std::string& channel,const std::string& userid,const std::string& pkg_name,
			const std::string& tag,const std::string& machine)
	:platform_(base::Value::CreateStringValue(platform))
	,uid_(base::Value::CreateStringValue(uid))
	,token_(base::Value::CreateStringValue(token))
	,channel_(base::Value::CreateStringValue(channel))
	,userid_(base::Value::CreateStringValue(userid))
	,pkg_name_(base::Value::CreateStringValue(pkg_name_))
	,tag_(base::Value::CreateStringValue(tag))
	,machine_(base::Value::CreateStringValue(machine)){
		this->Set("platform",platform_);
		this->Set("uid",uid_);
		this->Set("token",token_);
		this->Set("channel",channel_);
		this->Set("userid",userid_);
		this->Set("pkg_name",pkg_name_);
		this->Set("tag",tag_);
		this->Set("machine",machine_);
	}

private:
	NET_VALUE   platform_;
	NET_VALUE   uid_;
	NET_VALUE   token_;
	NET_VALUE   channel_;
	NET_VALUE   userid_;
	NET_VALUE   pkg_name_;
	NET_VALUE   tag_;
	NET_VALUE   machine_;
};

}



#endif /* COMM_HEAD_H_ */
